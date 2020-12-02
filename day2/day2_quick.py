import re


def match():
    with open('day2.in') as f:
        linez = [l.strip('\n') for l in f.readlines()]
    #print(linez)
    print(len(list(filter(is_ok, linez))))
    print(len(list(filter(is_ok2, linez))))


def is_ok(string):
    match_obj = re.match(r"(\d+)-(\d+) ([a-z]): ([a-z]+)", string)
    min_c = int(match_obj[1])
    max_c = int(match_obj[2])
    char = match_obj[3]
    payload = match_obj[4]
    count = len(list(filter(lambda x: x == char, payload)))
    return count <= max_c and count >= min_c

def is_ok2(string):
    match_obj = re.match(r"(\d+)-(\d+) ([a-z]): ([a-z]+)", string)
    first_c = int(match_obj[1]) - 1
    second_c = int(match_obj[2]) - 1
    char = match_obj[3]
    payload = match_obj[4]
    a = payload[first_c] == char
    b = payload[second_c] == char
    return  (a and not b) or (not a and b)

if __name__ == "__main__":
    match()