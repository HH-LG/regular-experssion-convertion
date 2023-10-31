import exrex
import random

def generate_regex():
    regex = ""
    for i in range(random.randint(5, 10)):   # 长度随机
        add_brac = random.randint(0, 1)     # 是否加括号
        add_or = random.randint(0, 1)       # 是否加or
        add_star = random.randint(0, 1)     # 是否加*

        s1 = exrex.getone("[a-z]{1,2}") # 生成随机长度的小写字母
        s2 = exrex.getone("[a-z]{1,2}") 

        if add_brac:
            if add_or:
                regex += "(" + s1 + "|" + s2 + ")"
            else:
                regex += "(" + s1 + ")"
        else:
            if add_or:
                regex += s1 + "|" + s2
            else:
                regex += s1

        if add_star:
            regex += "*"
    return regex

def generate_example(s):
    for i in range(100):
        print(exrex.getone(s))

regex = generate_regex()
print(regex)
generate_example(regex)