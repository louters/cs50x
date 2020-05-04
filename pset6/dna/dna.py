"""dna.py - Identify a person based on they DNA.
Usage: dna.py <DNA db> <sequence>"""
from sys import argv, exit
import csv
import re


def main():
    # Check for command prompt
    if len(argv) != 3:
        print("Usage: dna.py <DNA db.csv> <sequence.py>")
        exit(1)

    # Assign args to variables
    dna_db_file, seq_file = argv[1:3]

    # Open DNA database
    with open(dna_db_file, "r", newline="") as f:
        reader = csv.reader(f)
        dna_db = dict()
        for i, row in enumerate(reader):
            if i == 0:
                STRs = row[1:]
            else:
                dna_db[row[0]] = [int(x) for x in row[1:]]

    # Open sequence file
    with open(seq_file, "r") as f:
        seq = f.read()

    # Find longest consecutive run of each STR
    dna_id = [] 
    for STR in STRs:
        dna_id.append(longest_STR(STR, seq))

    # Compare the STR counts against each DNA db
    for name in dna_db:
        if dna_db[name] == dna_id:
            print(name)
            exit(0)
    print("No match")


def longest_STR(STR: str, seq: str) -> int:
    """Return the highest n of consecutive STR."""
    res = 0
    for i in range(len(seq)):
        if seq[i:i + len(STR)] == STR:
            tmp = 1
            for j in range(i + len(STR), len(seq) - len(STR), len(STR)):
                if seq[j:j + len(STR)] == STR:
                    tmp += 1
                else:
                    res = max(res, tmp)
                    break
    return res


if __name__ == "__main__":
    main()
