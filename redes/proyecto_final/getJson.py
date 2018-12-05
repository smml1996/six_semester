import json
import sys

id = sys.argv[1] # gets an index
txt = open("results.txt","r")

txt = txt.read()
try:
    d = json.loads(txt)

    writeFile = open("links"+ str(id)+".txt", "w")

    for hit in d["hits"]["hits"]:
        writeFile.write(hit["_source"]["link"])
        writeFile.write("\n")
except Exception:
    print("exception occured in python file")
