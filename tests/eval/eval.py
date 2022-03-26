import os
import requests
import sys

class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKCYAN = '\033[96m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

clear = lambda: os.system('clear')
clear()

def EvaluationTester():
    clear()
    print("##################################################################")
    print("#                        Evaluation Tester                       #")
    print("##################################################################")
    print("")

def TestGet(hoststring, status_code, _headers='', text=False):
    if _headers:
        print('HEADERS:     ', _headers)
    print('URL:          ' + hoststring)
    print('METHOD:       GET')

    if (_headers):
        r = requests.get(hoststring, headers=_headers)
    else:
        r = requests.get(hoststring)
    
    if status_code == 1:
        if r.status_code >= 400 and r.status_code <= 500:
            print('STATUS_CODE:', bcolors.FAIL, r.status_code, bcolors.ENDC)
        else:
            print('STATUS_CODE:', bcolors.OKGREEN, r.status_code, bcolors.ENDC)
    if text == True:
        print("")
        print(r.text)
    print("")


def TestPost(hoststring, status_code, _data='', text=False, _files=''):
    if _data:
        print('HEADERS:     ', _data)
    if _files:
        print('FILES:       ', _files)
    print('URL:          ' + hoststring)
    print('METHOD:       POST')

    if (_data):
        r = requests.post(hoststring, data=_data)
    elif (_files):
        r = requests.post(hoststring, files=_files)
    else:
        r = requests.post(hoststring)
    
    if status_code == 1:
        if r.status_code >= 400 and r.status_code <= 500:
            print('STATUS_CODE:', bcolors.FAIL, r.status_code, bcolors.ENDC)
        else:
            print('STATUS_CODE:', bcolors.OKGREEN, r.status_code, bcolors.ENDC)
    if text == True:
        print("")
        print(r.text)
    print("")


def TestDelete(hoststring, status_code, _data='', text=False):
    if _data:
        print('HEADERS:     ', _data)
    print('URL:          ' + hoststring)
    print('METHOD:       DELETE')

    if (_data):
        r = requests.delete(hoststring, data=_data)
    else:
        r = requests.delete(hoststring)
    
    if status_code == 1:
        if r.status_code >= 400 and r.status_code <= 500:
            print('STATUS_CODE:', bcolors.FAIL, r.status_code, bcolors.ENDC)
        else:
            print('STATUS_CODE:', bcolors.OKGREEN, r.status_code, bcolors.ENDC)
    if text == True:
        print("")
        print(r.text)
    print("")


def TestPatch(hoststring, status_code, text=False):
    print('URL:          ' + hoststring)
    print('METHOD:       PATCH')

    r = requests.patch(hoststring)
    
    if status_code == 1:
        if r.status_code >= 400 and r.status_code <= 500:
            print('STATUS_CODE:', bcolors.FAIL, r.status_code, bcolors.ENDC)
        else:
            print('STATUS_CODE:', bcolors.OKGREEN, r.status_code, bcolors.ENDC)
    if text == True:
        print("")
        print(r.text)
    print("")

def main():
    EvaluationTester()
    input("Press Enter to continue...")


    ##################################################################
    #                Multiple Server Different Ports                 #
    ##################################################################

    EvaluationTester()
    print("- Setup multiple servers with different port")
    print("")

    TestGet('http://localhost:8080', True)
    TestGet('http://localhost:8080/index.html', True)
    TestGet('http://localhost:8081', True)
    TestGet('http://localhost:8081/index1.html', True)
    TestGet('http://localhost:8082', True)
    TestGet('http://localhost:8082/index4.html', True)
    TestGet('http://localhost:8080/index2.html', True)
    TestGet('http://localhost:8081/index.html', True)


    input("Press Enter to continue...")

    ##################################################################
    #                Multiple Server Different Hosts                 #
    ##################################################################

    EvaluationTester()
    print("- Multiple Server Different Hosts")
    print("")

    headers = { }
    TestGet('http://localhost:8083', True, headers, True)

    input("Press Enter to continue...")
    EvaluationTester()
    print("- Multiple Server Different Hosts")
    print("")


    headers = { 'Host' : 'name' }
    TestGet('http://localhost:8083', True, headers, True)

    input("Press Enter to continue...")
    EvaluationTester()
    print("- Multiple Server Different Hosts")
    print("")

    headers = { 'Host' : 'other_name' }
    TestGet('http://localhost:8083', True, headers, True)

    input("Press Enter to continue...")
    EvaluationTester()
    print("- Multiple Server Different Hosts")
    print("")

    headers = { 'Host' : 'hostname' }
    TestGet('http://localhost:8083', True, headers, True)

    input("Press Enter to continue...")
    EvaluationTester()
    print("- Multiple Server Different Hosts")
    print("")



    headers = { }
    TestGet('http://localhost:8083', True, headers)

    headers = { 'Host' : 'name' }
    TestGet('http://localhost:8083', True, headers)

    headers = { 'Host' : 'other_name' }
    TestGet('http://localhost:8083', True, headers)

    headers = { 'Host' : 'hostname' }
    TestGet('http://localhost:8083', True, headers)


    input("Press Enter to continue...")


    ##################################################################
    #                         Error Page                             #
    ##################################################################

    EvaluationTester()
    print("- Error Page")
    print("")

    TestGet('http://localhost:8084/asdasd', True, '')
    TestGet('http://localhost:8084/zindex.html', True, '')

    input("Press Enter to continue...")


    ##################################################################
    #                         Body Size Limit                        #
    ##################################################################

    EvaluationTester()
    print("- Body Size Limit")
    print("")


    TestPost('http://localhost:8085', True, "1234567890")
    TestPost('http://localhost:8085', True, "12345678901")


    input("Press Enter to continue...")


    ##################################################################
    #                     Different Directories                      #
    ##################################################################

    EvaluationTester()
    print("- Different Directories")
    print("")

    TestGet('http://localhost:8086', True)
    TestGet('http://localhost:8086/blog', True)
    TestGet('http://localhost:8086/blog/blog2', True)

    input("Press Enter to continue...")


    ##################################################################
    #                           Indexes                              #
    ##################################################################

    EvaluationTester()
    print("- Indexes")
    print("")

    TestGet('http://localhost:8087', True)
    TestGet('http://localhost:8087/blog', True)
    TestGet('http://localhost:8087/blog/blog2', True)

    input("Press Enter to continue...")


    ##################################################################
    #                           Methods                              #
    ##################################################################

    EvaluationTester()
    print("- Methods")
    print("")

    TestGet('http://localhost:8088', True)
    TestPost('http://localhost:8088', True)
    TestDelete('http://localhost:8088', True)

    input("Press Enter to continue...")



    EvaluationTester()
    print("- Methods")
    print("")

    data = {'A Key': 'Value',
            'Another Key': 'Another Value'}

    TestGet('http://localhost:8089', True)
    TestPost('http://localhost:8089/print_vars.php', True, data, True)
    TestDelete('http://localhost:8089', True)

    input("Press Enter to continue...")



    EvaluationTester()
    print("- Methods")
    print("")

    TestGet('http://localhost:8090', True)

    input("Upload File...")
    files = {'upload_file': open('uploads/teste.html','rb')}
    TestPost('http://localhost:8090', True, "", False, files)
    files = {'upload_file': open('uploads/hello.txt','rb')}
    TestPost('http://localhost:8090', True, "", False, files)

    input("Delete File...")
    TestDelete('http://localhost:8090/teste.html', True)
    TestDelete('http://localhost:8090/hello.txt', True)

    input("Press Enter to continue...")


    ##################################################################
    #                       Unknown Request                          #
    ##################################################################

    EvaluationTester()
    print("- Unknown Request")
    print("")

    TestPatch('http://localhost:8091', True, True)

    input("Press Enter to continue...")

if __name__ == '__main__':
    try:
        main()
    except Exception as e:
        print(e)

        print("\nCHECK IF THE SERVER IS RUNNING!!!")
