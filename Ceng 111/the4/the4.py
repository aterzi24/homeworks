def check_commands(FS, C):
    wd = "/"
    error = 0
    chng(FS)
    FS.append("*")

    for i in C:
        arg = i.split()

        if arg[0] == "cd":
            if len(arg) > 2:
                error = 1
                break
            elif len(arg) == 1:
                wd = "/"
            else:
                wd, error = cdf(FS, wd, arg[1])
            if error == 1:
                break

        elif arg[0] == "mkdir":
            if len(arg) != 2:
                error = 1
                break
            if arg[1].strip("/") == "":
                error = 1
                break
            error = mkdf(FS, wd, arg[1])
            if error == 1:
                break

        elif arg[0] == "rmdir":
            if len(arg) != 2:
                error = 1
                break
            if arg[1].strip("/") == "":
                error = 1
                break
            error = rmdf(FS, wd, arg[1])
            if error == 1:
                break

        elif arg[0] == "rm":
            if len(arg) != 2:
                error = 1
                break
            error = rmf(FS, wd, arg[1])
            if error == 1:
                break

        elif arg[0] == "cp":
            if len(arg) != 3:
                error = 1
                break
            error = cpf(FS, wd, arg[1], arg[2])
            if error == 1:
                break

        elif arg[0] == "exec":
            if len(arg) != 2:
                error = 1
                break
            error = execf(FS, wd, arg[1])
            if error == 1:
                break

        else:
            error = 1
            break
    dechng(FS)
    FS.pop()

    if error == 1:
        return ("ERROR", i, wd)
    return ("SUCCESS", FS, wd)

def chng(FS):
    i = 2
    while i < len(FS):
        if FS[i][1] == "d" or FS[i][1] == "D":
            chng(FS[i])
            FS[i].append(FS)
        i += 1

def nwdf(FS, path):
    er = 0
    nwd = []
    for i in path:
        if i == "" or i == ".":
            continue
        if i == "..":
            if len(nwd) == 0:
                return nwd, 1
            FS = FS[-1]
            nwd.pop()
            continue
        er = 1
        j = 2
        while j < len(FS) - 1:
            if FS[j][0] == i and ( FS[j][1] == "d" or FS[j][1] == "D" ):
                FS = FS[j]
                nwd.append(FS[0])
                er = 0
                break
            j += 1
        if er == 1:
            return nwd, 1
    return nwd, 0

def cdf(FS, wd, path):
    if path[0] != "/":
        path = wd + "/" + path
    path = path.strip("/")
    path = path.split("/")
    nwd, er = nwdf(FS, path)
    if er == 1:
        return wd, 1
    return "/"+"/".join(nwd), 0

def mkdf(FS, wd, path):
    er = 0
    if path[0] != "/":
        path = wd + "/" + path
    path = path.strip("/")
    path = path.rsplit("/", 1)
    if len(path) == 1:
        trgt = path[0]
        nwd = []
    else:
        trgt = path[1]
        path = path[0]
        path = path.strip("/")
        path = path.split("/")
        nwd, er = nwdf(FS, path)
    if er == 1:
        return 1
    fxd = dirc(FS, nwd)
    i = 2
    while i < len(fxd) - 1:
        if fxd[i][0] == trgt:
            return 1
        i += 1
    fxd.insert(-1,[trgt,"d",fxd])
    return 0

def dirc(FS, nwd):
    for a in nwd:
        i = 2
        while i < len(FS) - 1:
            if FS[i][0] == a:
                FS = FS[i]
                break
            i += 1
    return FS

def rmdf(FS, wd, path):
    er  = 0
    if path[0] != "/":
        path = wd + "/" + path
    path = path.strip("/")
    path = path.rsplit("/", 1)
    if len(path) == 1:
        trgt = path[0]
        nwd = []
    else:
        trgt = path[1]
        path = path[0]
        path = path.strip("/")
        path = path.split("/")
        nwd, er = nwdf(FS, path)
    if er == 1:
        return 1
    fxd = dirc(FS, nwd)
    i = 2
    while i < len(fxd) - 1:
        if fxd[i][0] == trgt and (fxd[i][1] == "d" or fxd[i][1] == "D"):
            if len(fxd[i]) > 3:
                return 1
            nwd.append(trgt)
            wd = wd.strip("/")
            wd = wd.split("/")
            wdl, er = nwdf(FS, wd)
            if wdl == nwd:
                return 1
            fxd.pop(i)
            return 0
        i += 1
    return 1

def rmf(FS, wd, path):
    er = 0
    if path[0] != "/":
        path = wd + "/" + path
    path = path.strip("/")
    path = path.rsplit("/", 1)
    if len(path) == 1:
        trgt = path[0]
        nwd = []
    else:
        trgt = path[1]
        path = path[0]
        path = path.strip("/")
        path = path.split("/")
        nwd, er = nwdf(FS, path)
    if er == 1:
        return 1
    fxd = dirc(FS, nwd)
    i = 2
    while i < len(fxd) - 1:
        if fxd[i][0] == trgt and (fxd[i][1] == "f" or fxd[i][1] == "F"):
            fxd.pop(i)
            return 0
        i += 1
    return 1

def cpf(FS, wd, source, target):
    er = 0
    if source[0] != "/":
        source = wd + "/" + source
    source = source.strip("/")
    while source[-2:] == "/.":
        source = source[:-2]
        source = source.strip("/")
    source = source.rsplit("/", 1)
    if len(source) == 1:
        cpy = source[0]
        nwd = []
    elif source[1] == "..":
        nwd, er = nwdf(FS, source[0].split("/") + [".."])
        if len(nwd):
            cpy = nwd.pop()
    else:
        cpy = source[1]
        source = source[0]
        source = source.strip("/")
        source = source.split("/")
        nwd, er = nwdf(FS, source)
    if er == 1:
        return 1
    fxd = dirc(FS, nwd)

    if target[0] != "/":
        target = wd + "/" + target
    target = target.strip("/")
    while target[-2:] == "/.":
        target = target[:-2]
        target = target.strip("/")
    target = target.rsplit("/", 1)
    if len(target) == 1:
        pst = target[0]
        nwd2 = []
    elif target[1] == "..":
        nwd, er = nwdf(FS, target[0].split("/") + [".."])
        if len(nwd):
            pst = nwd.pop()
    else:
        pst = target[1]
        target = target[0]
        target = target.strip("/")
        target = target.split("/")
        nwd2, er = nwdf(FS, target)
    if er == 1:
        return 1
    fxd2 = dirc(FS, nwd2)

    i = 2
    while i < len(fxd2) - 1:
        if fxd2[i][0] == pst:
            if fxd2[i][1] == "f" or fxd2[i][1] == "F":
                return 1
            if fxd2[i][1] == "d" or fxd2[i][1] == "d":
                j = 2
                while j < len(fxd2[i]) - 1:
                    if fxd2[i][j][0] == cpy:
                        return 1
                    j += 1
                j = 2
                while j < len(fxd) - 1:
                    if fxd[j][0] == cpy:
                        afcp = copyf(fxd[j])
                        if fxd[j][1] == "d" or fxd[j][1] == "D":
                            afcp.append(fxd2[i])
                        fxd2[i].insert(-1, afcp)
                        return 0
                    j += 1
                return 1
        i += 1

    i = 2
    while i < len(fxd) - 1:
        if fxd[i][0] == cpy:
            afcp = copyf(fxd[i])
            if pst != "":
                afcp[0] = pst
            if afcp[1] == "d" or afcp[1] == "D":
                afcp.append(fxd2)
            fxd2.insert(-1, afcp)
            return 0
        i += 1
    return 1

def copyf(lst):
    if lst[1] == "f" or lst[1] == "F":
        return [lst[0], "f"]
    i = 0
    nwl = []
    while i < len(lst) - 1:
        if type(lst[i]) == list:
            t = copyf(lst[i])
            if t[1] == "d" or t[1] == "D":
                t.append(nwl)
            nwl.append(t)
        else:
            nwl.append(lst[i])
        i += 1
    return nwl

def execf(FS, wd, path):
    er = 0
    if path[0] != "/":
        path = wd + "/" + path
    path = path.strip("/")
    path = path.rsplit("/", 1)
    if len(path) == 1:
        trgt = path[0]
        nwd = []
    else:
        trgt = path[1]
        path = path[0]
        path = path.strip("/")
        path = path.split("/")
        nwd, er = nwdf(FS, path)
    if er == 1:
        return 1
    fxd = dirc(FS, nwd)
    i = 2
    while i < len(fxd) - 1:
        if fxd[i][0] == trgt and (fxd[i][1] == "f" or fxd[i][1] == "F"):
            return 0
        i += 1
    return 1

def dechng(FS):
    i = 2
    while i < len(FS) - 1:
        if FS[i][1] == "d" or FS[i][1] == "D":
            dechng(FS[i])
            FS[i].pop()
        i += 1
