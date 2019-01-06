import _pydxf
import uuid
import os


def read_text(path, codec="sjis"):
    lines = []
    with open(path, "r") as f:
        lines = f.readlines()

    temp_path = path + str(uuid.uuid4().int)
    with open(temp_path, "w") as f:
        for l in lines:
            if codec:
                try:
                    l = l.decode(codec).encode("utf8")
                except:
                    pass

            f.write(l)

    res = _pydxf.read_text(temp_path)
    os.remove(temp_path)

    return res
