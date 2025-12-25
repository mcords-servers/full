def prepare(filename):
    file = open(filename, "r")
    lines = []
    n = 0

    for line in file:
        n += 1
        line = line.strip()
        lines.append(line)
        # print(f"[{n}]", line)
    return lines

def parse(lines):
    for line in lines:
        l = [""]
        index = 0
        spaces = True

        for i in range(len(line)):
            c = line[i]
            if c == "\"" and (i>0 and line[i-1]!="\\"):
                spaces = not spaces
            elif c == " " and spaces:
                l.append("")
                index += 1
            else:
                l[index] += c

        print(l)


out = prepare("custom_lang/code.txt")
out = parse(out)