"""mario.py
Prints out a double half-pyramid of a specified height.
"""


def main():
    # Loop until acceptable heigth is provided
    while True:
        try:
            # Prompt user for height
            h = int(input("Height: "))

            # Test valid input 
            if h > 0 and h < 9:
                break
        except ValueError:
            pass
    
    # Print pyramid
    print_pyramid(h)


def print_pyramid(h: int) -> None:
    """ Print pyramid of <h> height"""
    for i in range(h, 0, -1):
        print(
            " " * (i - 1)
            + (h - i + 1) * "#"
            + "  "
            + (h - i + 1) * "#"
        )


if __name__ == "__main__":
    main()