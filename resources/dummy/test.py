import json
file = open("notes.json", "r").read() 

notes = json.loads(file)

count = 0;

for note in notes:
	if note['notebook'] == None:
		count += 1

print(count)
