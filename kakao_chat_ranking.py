def main():
    # 파일 오픈
    path = "C:\\Users\\tkddb.DESKTOP-JCAG9KP\\Desktop\\kakao.txt"
    f = open(path, "r", encoding='UTF-8')
    textlist = []

    chat = dict()

    # 파일 내용 1줄마다 체크
    while True:
        line = f.readline()
        if not line: break
        # 수직 탭을 삭제한다
        line = line.replace(u"\u000B", u"")

        line = line.replace("[", "").replace("\n", "").replace(" ", "")
        textlist = line.split("]")
        if (len(textlist) == 3):
            # print(textlist[2])
            if (textlist[2] in chat):
                value = chat.get(textlist[2]) + 1
                chat[textlist[2]] = value
            else:
                chat[textlist[2]] = 1

    #print(chat)
    #print(list(chat.keys()))
    chatkeylist = list(chat.keys())
    i = 0

    while (i < len(chatkeylist)):
        if (chat.get(chatkeylist[i]) < 5):
            del (chat[chatkeylist[i]])
        i = i + 1

    #print(chat.items())
    sortedchat = sorted(chat.items(), reverse=True, key=lambda item: item[1])
    print(sortedchat)

    f.close()


main()
