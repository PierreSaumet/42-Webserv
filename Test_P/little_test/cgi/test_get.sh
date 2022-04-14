#!/bin/sh
REQUEST_DATA="nom=saumet&prenom=pierre"
export GATEWAY_INTERFACE="CGI/1.1"			# fonctionne sans
export SERVER_PROTOCOL="HTTP/1.1"			# fonctionne sans
export QUERY_STRING="name=PIERRE"		# fonctionne sans
export REDIRECT_STATUS="200"
export SCRIPT_FILENAME="./test_get.php"
export REQUEST_METHOD="GET"
export CONTENT_LENGTH=${#REQUEST_DATA}
export CONTENT_TYPE="application/x-www-form-urlencoded;charset=utf-8"


echo $REQUEST_DATA | /usr/bin/php-cgi


################  sh test_get.sh ==> ca retour ca 

# Array
# (
#     [name] => PIERRE
# )

