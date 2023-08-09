from cs50 import get_string
text = get_string("Text: ")

letters = 0
words = 1
sentences = 0
for i in text:
    if 'a' <= i <= 'z' or 'A' <= i <= 'Z':
        letters += 1
    if i == ' ': # or '
        words += 1
    if i == '.' or i == '?' or i == '!':
        sentences += 1

L = letters / words * 100
S = sentences / words * 100
grade = 0.0588 * L - 0.296 * S - 15.8

if grade < 1:
    print("Before Grade 1")
elif grade >= 16:
    print("Grade 16+")
else:
    print(f"Grade: {int(grade)}")