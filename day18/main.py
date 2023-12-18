with open("input.txt", "r") as f:
    line = f.readline()
    perimeter = 0
    current = [0,0]
    coords = [current,]

    ## Part 1
    #while (line != ""):

    #    dir = line[0:1]
    #    l = int(line[2:4])
    #    if dir == "U":
    #        current[1] = current[1] - l
    #    if dir == "L":
    #        current[0] = current[0] - l
    #    if dir == "D":
    #        current[1] = current[1] + l
    #    if dir == "R":
    #        current[0] = current[0] + l

    #    perimeter += l

    #    print(f"Now at {current}")
    #    coords.append(current[:])
    #    line = f.readline()

    while (line != ""):

        dir = line.split(" ")[-1][7:8]
        l = int(line.split(" ")[-1][2:7], 16)
        if dir == "3":
            print(f"U {l}")
            current[1] = current[1] - l
        if dir == "2":
            print(f"L {l}")
            current[0] = current[0] - l
        if dir == "1":
            print(f"D {l}")
            current[1] = current[1] + l
        if dir == "0":
            print(f"R {l}")
            current[0] = current[0] + l

        perimeter += l

        print(f"Now at {current}")
        coords.append(current[:])
        line = f.readline()

    n = len(coords)
    coords.insert(0, coords[n - 1])
    coords.insert(n+1, coords[1])

    shoelace = 0
    for i in range(1, n + 1):
        shoelace += coords[i][1]*(coords[i-1][0] - coords[i+1][0])

    area = shoelace//2 + perimeter//2 + 1
    print(f"The answer is {area}")
