"""readability.py
Computes the approximate grade level needed to comprehend some text."""
import re


def main():
    # Prompt user for input text
    text = input("Text: ")

    # Count letters, words, sentences in text
    l = count_letters(text)
    w = count_words(text)
    s = count_sentences(text)

    # Compute mean n of letters per 100 words
    L = 100 * l / w

    # Compute mean n of sentences per 100 words
    S = 100 * s / w

    # Compute Coleman-Liau index
    idx = round(0.0588 * L - 0.296 * S - 15.8)

    # Print grade readability
    if idx < 1:
        print("Before Grade 1")
    elif idx > 16:
        print("Grade 16+")
    else:
        print(f"Grade {idx}")
    

def count_letters(text: str) -> int:
    """Return n of letters in <text>"""
    return len(re.findall("([a-zA-Z])", text))


def count_words(text: str) -> int:
    """Return n of words in <text>"""
    return len(text.split())


def count_sentences(text: str) -> int:
    """Return n of sentences in <text>"""
    return len(re.findall("([.|!|?])", text))


if __name__ == "__main__":
    main()