import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

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
    # Display symbols and sum of shares from transactions table
    portfolio = db.execute("SELECT symbol, SUM(shares) AS sum_shares FROM transactions WHERE user_id = ? GROUP BY symbol HAVING SUM(shares) > 0", session["user_id"])

    for i, item in enumerate(portfolio):
        quote = lookup(item.get("symbol"))
        item["name"] = quote["name"]
        item["price"] = quote["price"]
        portfolio[i] = item

    # Cash, current cash balance
    cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0].get("cash")

    # Total, grand total (total value of stocks + cash)
    total = cash
    for item in portfolio:
        total += item["price"] * item["sum_shares"]

    # Return render_template("buyed.html", name=name)
    return render_template("index.html", portfolio=portfolio, cash=cash, total=total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "GET":
        return render_template("buy.html")

    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")
        quote = lookup(symbol)
        user_id = session["user_id"]

        # Error checkings
        if not symbol or quote is None:
            return apology("invalid symbol")

        symbol = quote.get("symbol")

        if not shares or int(shares) < 1:
            return apology("invalid shares")

        # SELECT how much cash the user currently has in users
        price = quote.get("price")
        cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0].get("cash")

        # If user cannot afford, return apology
        if (price * int(shares)) > cash:
            return apology("unsufficient cash")

        # Update cash to users table
        db.execute("UPDATE users SET cash = ? WHERE id = ?", cash - (price * int(shares)), user_id)

        # Insert to table
        db.execute("INSERT INTO transactions (user_id, symbol, shares, price) VALUES (?, ?, ?, ?)", user_id, symbol, shares, price)

        # Render user to index page
        return redirect("/")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    transactions = db.execute("SELECT * FROM transactions WHERE user_id = ?", session["user_id"])

    return render_template("history.html", transactions=transactions)


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
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

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
    """Get stock quote."""
    if request.method == "GET":
        # Display a form to request a stock quote
        return render_template("quote.html")

    if request.method == "POST":
        symbol = request.form.get("symbol")
        quote = lookup(symbol)
        if quote != None:
            return render_template("quoted.html", quote=quote)
        # If user make typo, the python value would be none, print out an error message
        else:
            return apology("invalid symbol")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "GET":
        return render_template("register.html")

    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        # Render apology if username is blank
        if not username:
            return apology("username needed")

        # Render apology if username is already exist
        if db.execute("SELECT username FROM users WHERE username = ?", username):
            return apology("username already exist")

        # Render apology if password and/or confirmation is blank or dont match
        if not password or not confirmation or password != confirmation:
            return apology("invalid password/passwords doesn't match")

        # Require users’ passwords to have some number of letters and numbers
        if len(password) < 6:
            return apology("must contain 6 or more characters")

        digits = 0
        letters = 0
        for c in password:
            if c.isdigit():
                digits += 1
            if c.isalpha():
                letters += 1

        if digits == 0 or letters == 0:
            return apology("must contain one or more letters/digits")

        # Insert user into db, hash password
        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username, generate_password_hash(password))
        # Bring log in page
        return redirect("/login")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    user_id = session["user_id"]
    # Get a list of dict of symbols
    symbols = db.execute("SELECT symbol FROM transactions WHERE user_id = ? GROUP BY symbol HAVING SUM(shares) > 0", user_id)
    # Make it a simple list
    symbols = [d["symbol"] for d in symbols]
    # Eliminate duplicates by converting it to a set
    symbols = set(symbols)

    if request.method == "GET":
        return render_template("sell.html", symbols=symbols)

    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        # Return apology if user do not choose a symbol or do not have any shares of chosen symbol
        if not symbol:
            return apology("missing symbol")
        if symbol not in symbols:
            return apology("invalid symbol")

        if not shares or not shares.isdigit() or int(shares) < 1:
            return apology("invalid shares")

        # Return apology if shares is missing or user do not have that many of shares
        shares_sum = db.execute("SELECT SUM(shares) FROM transactions WHERE symbol = ? AND user_id = ?", symbol, user_id)
        shares_sum = shares_sum[0]['SUM(shares)']
        if int(shares) > shares_sum:
            return apology("invalid shares")

        # See current price and calculate money amount
        price = lookup(symbol).get("price")
        amount = price * int(shares)

        # Add amount, update cash to users table, insert transactions table
        cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
        cash = cash[0]['cash'] + amount
        db.execute("UPDATE users SET cash = ? WHERE id = ?", cash, user_id)
        shares = 0 - int(shares)
        db.execute("INSERT INTO transactions (user_id, symbol, shares, price) VALUES (?, ?, ?, ?)", user_id, symbol, shares, price)

        # Return index
        return redirect("/")

@app.route("/changepw", methods=["GET", "POST"])
def changepw():
    """Allow user to change password"""
    if request.method == "GET":
        return render_template("changepw.html")

    if request.method == "POST":
        old_password = request.form.get("old_password")
        new_password = request.form.get("new_password")
        confirmation = request.form.get("confirmation")

        # Return apology if there is a blank area
        if not old_password or not new_password or not confirmation:
            return apology("missing information")

        # Return apology if old password is wrong
        hash_from_table = db.execute("SELECT hash FROM users WHERE id = ?", session["user_id"])[0]["hash"]
        if not check_password_hash(hash_from_table, old_password):
            return apology("old password is wrong")

        # Return apology if new password and confirmation does not match
        if new_password != confirmation:
            return apology("passwords doesn't match")

        # Require users’ passwords to have some number of letters and numbers
        if len(new_password) < 6:
            return apology("must contain 6 or more characters")

        digits = 0
        letters = 0
        for c in new_password:
            if c.isdigit():
                digits += 1
            if c.isalpha():
                letters += 1

        if digits == 0 or letters == 0:
            return apology("must contain one or more letters/digits")

        # Insert user into database, hash password
        db.execute("UPDATE users SET hash = ? WHERE id = ?", generate_password_hash(new_password), session["user_id"])
        # Bring log in page
        return redirect("/")

def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)

# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
