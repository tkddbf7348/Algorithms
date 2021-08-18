import requests
import csv
import json
import time
import socket
import sys
from bs4 import BeautifulSoup
from collections import OrderedDict


def main() :
    url_Jobkorea = 'https://www.saramin.co.kr/zf_user/company-search?searchWord=%ED%95%B4%EC%96%91&area=&industry=&welfare=&scale=&listingType=&companyType=&employees=&revenue=&operatingRevenue=&netRevenue=&establishment=&salary=&startingSalary=&order=&recruitCheck=&page='

    company = [] #[ [회사이름, 회사정보url] ......]
    companyInfo = [] #회사 정보(업종, 대표자명, 홈페이지)

    for i in range(1, 10) :#끝:97
        tmp_url_Jobkorea = url_Jobkorea + str(i)
        print("접속 URL : ", tmp_url_Jobkorea)

        res = requests.get(tmp_url_Jobkorea)
        soup = BeautifulSoup(res.content, 'html.parser')

        nameList = soup.find_all(attrs={'class':'area_info'})
        for i in nameList :
            company.append([i.a.string, "https://www.saramin.co.kr" + i.find("a")["href"]])

    print(company)


if(__name__ == "__main__") :
    main()