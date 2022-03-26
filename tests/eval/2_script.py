import requests
import sys

headers = { 'Host' : 'cima',
			'Connection' : 'Keep-Alive',
			'Accept-Language' : 'en-US,en;q=0.5',
			'User-Agent' : 'python script' }

r = requests.get('http://127.0.0.1:' + sys.argv[1], headers=headers)

print("")
print("")
print('URL: ', r.url)
print('STATUS_CODE: ', r.status_code)
print('HEADERS: ', r.headers)
print("")
print('TEXT: ', r.text)
print("")
print("")
