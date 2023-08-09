-- Keep a log of any SQL queries you execute as you solve the mystery.
-- Step 1: Investigate description from crime_scene_reports to find data about case.
SELECT description FROM crime_scene_reports WHERE year = 2021 AND month = 7 and day = 28 AND street = "Humphrey Street";

-- Step 2: Look at the witness's testimony of that day.
SELECT name, transcript FROM interviews WHERE year = 2021 AND month = 7 and day = 28;

-- Step 3: Figure out the earliest plane went to which city.
SELECT city FROM airports
JOIN flights ON destination_airport_id = airports.id
WHERE year = 2021 AND month = 7 AND day = 29 ORDER BY hour, minute LIMIT 1;
-- So the thief escaped to New York City.

-- Step 4: Combine all the information received from witness's.
SELECT name FROM people
JOIN bakery_security_logs ON bakery_security_logs.license_plate = people.license_plate
WHERE year = 2021 AND month = 7 AND day = 28 AND activity = 'exit' AND 25 > minute AND minute > 15
INTERSECT
SELECT name FROM people
JOIN bank_accounts ON bank_accounts.person_id = people.id
JOIN atm_transactions ON atm_transactions.account_number = bank_accounts.account_number
WHERE year = 2021 AND month = 7 AND day = 28 AND atm_location = 'Leggett Street' AND transaction_type = 'withdraw'
INTERSECT
SELECT name FROM people
JOIN passengers ON passengers.passport_number = people.passport_number
WHERE passengers.flight_id = (SELECT id FROM flights
WHERE year = 2021 AND month = 7 AND day = 29 ORDER BY hour, minute)
INTERSECT
SELECT name FROM people
JOIN phone_calls ON phone_calls.caller = people.phone_number
WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60;
-- The thief is Bruce.

-- Step 5: Find accomplice regarding who received a from call from the thief at crime day.
SELECT name FROM people
JOIN phone_calls ON phone_calls.receiver = people.phone_number
WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60 AND caller = (
SELECT phone_number FROM people
WHERE people.name = 'Bruce');
-- The accomplice is Robin.