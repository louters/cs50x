"""credit.py
Determines whether a provided credit card number is valid according to Luhn’s
algorithm."""


def main():
    # Prompt for input
    n = input("Number: ")

    msg = "INVALID"
    if checksum(n):
        # Which CC?
        msg = which_CC(n)
    
    print(msg)


def checksum(n: str) -> bool:
    """ Perform Lunh's algo. Return True if checksum."""
    res = 0

    for i, digit in enumerate(reversed(n), start=1):
        digit = int(digit)
        # Every other digit
        if i % 2 == 0:
            digit *= 2
            res += digit % 10
            res += digit // 10
        else:
            res += digit

    # Return true if last digit of res is 0
    if res % 10 == 0:
        return True
    return False


def which_CC(n: str) -> str:
    """Return credit card type or INVALID"""
    
    # Amex starts with 34 or 37 and has 15 digits
    if len(n) == 15:
        if n[:2] == "34" or n[:2] == "37":
            return "AMEX"
    
    # MC starts with 51-55 and has 16 digits
    if len(n) == 16:
        if int(n[:2]) > 50 and int(n[:2]) < 56:
            return "MASTERCARD"
    
    # VISA starts with 4 and has 13 or 16 digits
    if len(n) == 13 or len(n) == 16:
        if n[0] == "4":
            return "VISA"
    
    # Did not match any card
    return "INVALID"


if __name__ == "__main__":
    main()