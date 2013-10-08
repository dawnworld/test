import sys, os, json, shutil
dic = {
         'str': 'this is a string',
         'list': [1, 2, 'a', 'b'],
         'sub_dic': {
                       'sub_str': 'this is sub str',
                       'sub_list': [1, 2, 3]
                     },
         'end': 'end'
       }
json_str = json.dumps(dic)
print json_str
result = shutil.rmtree('/Users/mac/Downloads/SocksiPy')
print result