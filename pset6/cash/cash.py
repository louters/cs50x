"""cash.py
Calculates the minimum number of coins required to give a user change."""


def main():
    while True:
        try:
            # Prompt user
            owed = float(input("Change owed: "))
            # Check non-negative
            if owed >= 0:
                break
        except ValueError:
            pass
    
    # Convert to cents
    cents = round(owed * 100)

    # Compute min of coins required
    res = min_coins(cents)

    # Print result
    print(res)


def min_coins(n: float) -> int:
    """ Compute min of coins possible on owned cents"""
    cnt = 0
    while (n > 0):
        if n >= 25:
            n -= 25
        elif n >= 10:
            n -= 10
        elif n >= 5:
            n -= 5
        else:
            n -= 1
        cnt += 1
    return cnt


if __name__ == "__main__":
    main()
