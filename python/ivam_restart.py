import os
import pycurl
import StringIO

html = StringIO.StringIO()
url = r'https://mail.qq.com/cgi-bin/loginpage'
c = pycurl.Curl()
c.setopt(pycurl.URL,url)
c.setopt(pycurl.SSL_VERIFYHOST, False)
c.setopt(pycurl.SSL_VERIFYPEER,False)
#c.setopt(pycurl.USERAGENT,r"User-Agent: Dalvik/1.4.0 (Linux; U; Android 2.3.7; Milestone Build/SHOLS_U2_05.26.3)")
c.setopt(pycurl.WRITEFUNCTION, html.write)
c.setopt(pycurl.FOLLOWLOCATION, 1)
c.perform()
print c.getinfo(pycurl.HTTP_CODE), c.getinfo(pycurl.EFFECTIVE_URL)
print html.getvalue()