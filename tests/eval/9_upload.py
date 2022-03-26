import requests
import sys

#python3 7_methods_correct_8081.py 8082

headers = { 'Connection' : 'Keep-Alive',
			'Accept-Language' : 'en-US,en;q=0.5',
			'User-Agent' : 'python script' }

files = {'upload_file': open('teste.html','rb')}

values = {'DB': 'photcat', 'OUT': 'csv', 'SHORT': 'short'}


r = requests.post('http://127.0.0.1:' + sys.argv[1], files=files, data=values)

print("")
print("")
print('URL: ', r.url)
print('STATUS_CODE: ', r.status_code)
print('HEADERS: ', r.headers)
print("")
print('TEXT: ', r.text)
print("")
print("")

