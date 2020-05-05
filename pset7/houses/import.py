from sys import argv
import csv
import sqlite3


def main():
    # Control command prompt 
    if len(argv) != 2:
        print("Usage: python import.py <csv file>")
        exit(1)

    # Create empty db
    open(f"students.db", "w").close()

    # Connect DB & create cursor
    conn = sqlite3.connect("students.db")
    c = conn.cursor()

    # Create students table
    c.execute("CREATE TABLE students (id INTEGER PRIMARY KEY AUTOINCREMENT, first VARCHAR(255), middle VARCHAR(255), last VARCHAR(255), house VARCHAR(10), birth INTEGER)")
    
    # Read CSV and insert data into DB
    with open(argv[1], 'r') as f:
        # Create DictReader
        reader = csv.DictReader(f)

        # Iterate over CSV file
        for row in reader:
            # Student's full name
            full_name = [None] * 3
            full_name[0] = row["name"].split()[0]
            full_name[2] = row["name"].split()[-1]
            # Treat middle name
            if len(row["name"].split()) == 3:
                full_name[1] = row["name"].split()[1]
            
            # SQL Query
            q = "INSERT INTO students (first, middle, last, house, birth) VALUES (?, ?, ?, ?, ?)"
            # Parameters for SQL Query
            p = [full_name[0], full_name[1], full_name[2], row["house"], int(row["birth"])]
            # Execute SQL query
            c.execute(q, p)

    # Save changes
    conn.commit()
    # Close cursor 
    c.close()
    # Close connection
    conn.close()


if __name__ == "__main__":
    main()