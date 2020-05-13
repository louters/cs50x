import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # User ID
    id = session["user_id"]

    # Available cash
    cash = db.execute("SELECT cash FROM users WHERE id=:id", id=id)[0]["cash"]

    # Portfolio
    ptf = {}
    total = 0
    try:
        rows = db.execute("SELECT * FROM portfolios WHERE id=:id", id=id)
        for row in rows:
            if row["symbol"] not in ptf:
                ptf[row["symbol"]] =  {
                    "name": row["name"],
                    "shares": row["shares"],
                    "price": lookup(row["symbol"])["price"]
                    }
            else:
                ptf[row["symbol"]]["shares"] += row["shares"]

        # Format ptf USD values for index.html
        for symbol in ptf:
            total += ptf[symbol]["shares"] * ptf[symbol]["price"]
            ptf[symbol]["total"] = usd(ptf[symbol]["shares"] * ptf[symbol]["price"])
            ptf[symbol]["price"] = usd(ptf[symbol]["price"])

    finally:
        total += cash
        total = usd(total)
        ptf["CASH"] = {
                    "name": "",
                    "shares": "",
                    "price": "",
                    "total": usd(cash)
                    }
        return render_template("index.html", total=total, ptf=ptf)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # User reached route via GET
    if request.method == "GET":
        return render_template("buy.html")

    # User reached route via POST
    else:
        symbol = request.form.get("symbol")
        shares = int(request.form.get("shares"))

        # Check for non-empty inputs
        if not shares or not symbol.strip():
            return apology("Provide symbol and number of shares.")

        #Result of API request
        res = lookup(symbol)

        # Symbol does not exist
        if res is None:
            return apology(f"{symbol} does not exist.")

        price = res["price"]
        cash = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])[0]["cash"]

        # Check enough cash for oder
        if shares * price > cash:
            return apology(f"You do not have {usd(shares*res['price'])} to buy {symbol} ({usd(price)})")

        # Check if table portflio exists
        sql = """
            CREATE TABLE IF NOT EXISTS portfolios (
            id INTEGER NOT NULL,
            symbol VARCHAR(255),
            name VARCHAR(255),
            shares INTEGER UNSIGNED,
            FOREIGN KEY(id) REFERENCES users(id))"""
        db.execute(sql)

        # Check if table history exists
        sql = """
            CREATE TABLE IF NOT EXISTS history (
            id INTEGER NOT NULL,
            symbol VARCHAR(255),
            shares INTEGER UNSIGNED,
            price DECIMAL(10, 2),
            transacted DATETIME,
            FOREIGN KEY(id) REFERENCES users(id))"""
        db.execute(sql)

        # Insert transaction into history
        sql = """
            INSERT INTO history (id, symbol, shares, price, transacted)
            VALUES (:id, :symbol, :shares, :price, :transacted)"""
        db.execute(sql, id=session["user_id"], symbol=symbol, shares=shares, price=price, transacted=datetime.now())

        # Update cash available
        sql = """
            UPDATE users
            SET cash=:amount
            WHERE id=:id
            """
        db.execute(sql, id=session["user_id"], amount=(cash-shares*price))

        # Insert share into portfolio
        sql = "SELECT symbol FROM portfolios WHERE id=:id AND symbol=:symbol"
        if not db.execute(sql, id=session["user_id"], symbol=symbol):
            sql = """
                INSERT INTO portfolios (id, symbol, name, shares)
                VALUES (:id, :symbol, :name, :shares)"""
            db.execute(sql, id=session["user_id"], symbol=symbol, name=res["name"], shares=shares)
        # Update shares into portfolio
        else:
            shares += db.execute("""
                SELECT shares FROM portfolios WHERE id=:id AND symbol=:symbol
                """, id=session["user_id"], symbol=symbol)[0]["shares"]

            sql = "UPDATE portfolios SET shares=:shares WHERE id=:id AND symbol=:symbol"
            db.execute(sql, shares=shares, id=session["user_id"], symbol=symbol)

        return redirect("/")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # Query all transactions from user
    sql = "SELECT * FROM history WHERE id=:id"
    trans = db.execute(sql, id=session["user_id"])

    # Format price
    for tran in trans:
        tran["price"] = usd(tran["price"])

    return render_template("history.html", trans=trans)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]
        session["username"] = request.form.get("username")

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Lookup a quote"""
    # GET method
    if request.method == "GET":
        return render_template("quote.html")

    # POST method
    else:
        symbol = request.form.get("symbol")
        # Check symbol is not empty
        if symbol.strip() == "":
            return apology("Please provide a symbol")

        res = lookup(symbol)
        return render_template("quoted.html", name=res["name"], price=res["price"], symbol=res["symbol"])


@app.route("/new_pwd", methods=["GET", "POST"])
@login_required
def change_password():
    """Let existing user change his password"""
    # GET method
    if request.method == "GET":
        return render_template("new_pwd.html")
    # POST method
    else:
        old_pwd = request.form.get("old_pwd")
        new_pwd = request.form.get("new_pwd")
        conf = request.form.get("conf")

        # Check old pwd is correct
        tmp = db.execute("SELECT hash FROM users WHERE id=:id", id=session["user_id"])[0]["hash"]
        if not check_password_hash(tmp, old_pwd):
            return apology("Your password is not correct")

        # Check new pwd and conf match
        if new_pwd != conf:
            return apology("New passowrd and confirmation do not match")

        # Update new pwd
        sql = "UPDATE users SET hash=:hash WHERE id=:id"
        hash = generate_password_hash(new_pwd)
        db.execute(sql, id=session["user_id"], hash=hash)

        # Clear session and redirect to login page
        return redirect("/logout")

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # GET method for user to register
    if request.method == "GET":
        return render_template("register.html")
    # POST method for user's data to be checked and sent to db
    else:
        username = request.form.get("username")
        pwd = request.form.get("password")
        conf = request.form.get("confirmation")

        # Username is empty string
        if username.strip() == "":
            return apology("Username cannot be empty")

        # Username already exists
        if db.execute("SELECT username FROM users WHERE username=:username", username=username):
            return apology("Username already exists")

        # Check for pwd
        if pwd != conf or pwd.strip() == "":
            return apology("Ensure password and confirmation match and that password is not empty")

        # Insert user in db
        sql = "INSERT INTO users (username, hash) VALUES (:username, :hash)"
        db.execute(sql, username=username, hash=generate_password_hash(pwd))

        return redirect("/")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    # Get portfolio data
    sql = """
        SELECT symbol, SUM(shares) AS shares FROM portfolios
        WHERE id=:id GROUP BY symbol
        """
    ptf = db.execute(sql, id=session["user_id"])
    symbols = {x["symbol"] for x in ptf}
    ptf = {row["symbol"]:row["shares"] for row in ptf}

    # GET method for user to sell shares
    if request.method == "GET":
        return render_template("sell.html", symbols=symbols)

    # POST method for user to submit shares he wants to sell
    else:
        # Check symbol
        symbol = request.form.get("symbol")
        if symbol == "" or symbol not in symbols:
            return apology(f"{symbol} is not in your portfolio.")

        # Check shares input
        shares = int(request.form.get("shares"))
        if shares == "" or shares <= 0:
            return apology("Provide with a non-null positive integer for shares.")

        # Check shares available in ptf
        if shares > ptf[symbol]:
            return apology(f"You do not have {shares} of {symbol}.")

        # Price of shares
        price = lookup(symbol)["price"]

        # Update shares in portfolio
        sql = "SELECT shares FROM portfolios WHERE symbol=:symbol AND id=:id"

        "New amouth of shares"
        new_shares = ptf[symbol] - shares

        if new_shares == 0:
            # Delete row given zero shares left
            sql = "DELETE FROM portfolios WHERE id=:id AND symbol=:symbol"
            db.execute(sql, id=session["user_id"], symbol=symbol)
        else:
            # Update row
            sql = "UPDATE portfolios SET shares=:shares WHERE id=:id AND symbol=:symbol"
            db.execute(sql, shares=new_shares, id=session["user_id"], symbol=symbol)

        # Update cash in users
        cash = db.execute("SELECT cash FROM users WHERE id=:id", id=session["user_id"])[0]["cash"]
        cash += shares * price
        db.execute("UPDATE users SET cash=:cash WHERE id=:id", cash=cash, id=session["user_id"])

        # Update history
        sql = """
            INSERT INTO history (id, symbol, shares, price, transacted)
            VALUES (:id, :symbol, :shares, :price, :transacted)"""
        db.execute(sql, id=session["user_id"], symbol=symbol, shares=-shares, price=price, transacted=datetime.now())

        return redirect("/")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
