#! /bin/sh
#
# replace.sh
# Copyright (C) 2015 qiyl <qiyl@certusnet.com.cn>
#
# Distributed under terms of the CERTUSNET license.
#


original_string='-Werror -Wall'
string_to_replace_Suzi_with=Sara
result_string="${original_string/-Werror/}"
echo $result_string
