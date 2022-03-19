import requests
import sys

headers = { 'Host' : 'this_one',
			'Connection' : 'Keep-Alive',
			'Accept-Language' : 'en-US,en;q=0.5',
			'User-Agent' : 'python script' }

data={}
data['id'] = "87440"
data['type']="page"
data['title']="Data Page"

r = requests.delete('http://127.0.0.1:' + sys.argv[1], json=data, headers=headers)

print("")
print("")
print('URL: ', r.url)
print('STATUS_CODE: ', r.status_code)
print('HEADERS: ', r.headers)
print("")
print('TEXT: ', r.text)
print("")
print("")

#print(dir(r))
