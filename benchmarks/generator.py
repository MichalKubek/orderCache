import argparse
import sys
import random


def generate(args):
    companies = args.companies
    users = args.users
    instruments = args.instruments
    sides = ["Buy", "Sell"]
    for i in range(1, args.orders + 1):
        company = random.randint(1, companies + 1)
        user = random.randint(1, users + 1)
        instrument = random.randint(1, instruments + 1)
        args.output.write(",".join([
            f"OrderId{i}",
            f"SecId{instrument}",
            random.choice(sides),
            str(random.randint(10, 10_000_000)),
            f"UserId{user}",
            f"Company{company}",
        ]) + "\n")


def get_args(args=None):
    parser = argparse.ArgumentParser(description='Order generator')
    parser.add_argument('--companies', type=int, help='The number of companies to generate', default=10)
    parser.add_argument('--users', type=int, help='The number of users to generate per company', default=10)
    parser.add_argument('--output', help='The output file to write the data', type=argparse.FileType('w'), default=sys.stdout)
    parser.add_argument('--instruments', type=int, help='The number of instruments to generate', default=10)
    parser.add_argument('--orders', type=int, help='The number of orders to generate', required=True)

    return parser.parse_args(args=args)


def main():
    args = get_args()
    generate(args)


if __name__ == '__main__':
    main()
