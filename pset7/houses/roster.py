from sys import exit, argv
import sqlite3


def main():
    # Check for command prompt
    if len(argv) != 2:
        print("Usage: python roster.py <house>")
        exit(1)
    
    # House
    house = [argv[1]]

    # Connect to students DB
    conn = sqlite3.connect("students.db")
    c = conn.cursor()

    # Query
    q = "SELECT * FROM students WHERE house = ? ORDER BY last, first"
    # Execute query
    c.execute(q, house)
    roster = c.fetchall()

    # Print roster
    for r in roster:
        print(f"{r[1]}", end=' ')
        if r[2]:
            print(f"{r[2]}", end=' ')
        print(f"{r[3]},", end=' ')
        print(f"born {r[5]}")
        
    # Close db
    c.close()
    conn.close()


if __name__ == "__main__":
    main()