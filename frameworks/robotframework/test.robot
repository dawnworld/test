*** Settings ***
Library    Telnet      default_log_level=DEBUG
Library    UmgmdLibrary.py

*** Keywords ***
Login Dss
                [Arguments]         ${ip}
                Open Connection     ${ip}       port=8888
                Set Newline         \n
                Set Prompt          [ADMIN]
                Execute Command     show
                Sleep               2
    ${out}      Read
                Log                 ${out}
    ${out}      test
                Log                 ${out}

*** Test Cases ***
Example
    Login Dss           172.16.16.143

