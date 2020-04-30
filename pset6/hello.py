"""hello.py
Program that prompts a user for their name, and then prints hello, so-and-so,
where so-and-so is their provided name"""


def main():
    # Prompt user for name
    name = input("What is your name?\n")
    print(f"hello, {name}")


# Boilerplate main
if __name__ == "__main__":
    main()