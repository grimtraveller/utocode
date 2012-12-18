import json
d = dict()
d['key0'] = 'value0'
d['key1'] = 'value1'
f = open('data.json', 'wb')
f.write(json.dumps(d))
f.close()
obj = json.loads(open('data.json').read())
print type(obj)
print obj
