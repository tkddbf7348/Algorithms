#global
start = ['1', '4', '3', '7', ' ', '6', '5', '8', '2']
goal = ['1', '3', '6', '7', '4', '2', '5', '8', ' ']
tmp_route = [] #지나온 경로 저장
route = [] # 최단 경로 저장
    #[방향, 퍼즐]
    #최소 경로로 도달하기 위한 방향 삽입( 0: 위쪽 1 : 오른쪽 2 : 아래쪽 3 : 왼쪽)
visited = 0 # 다시 돌아가는 것 방지
depth = 0 #BFS 깊이

def print_puzzle(puzzle) : # 현재 퍼즐 출력
    for i in enumerate(puzzle) :
        print(i[1] + ' ', end=' ')
        if(i[0] % 3 == 2) :
            print()
    print()

def F(puzzle, goal, depth) : #퍼즐 f값 계산

    f = 0 #f값
    # 목표 예상 값(=목표와 다른 노드 수)
    cnt = 0 
            
    for i in enumerate(puzzle) : # i = [idx, value]
        if(i[1] != ' ' and i[1] != goal[i[0]]) : #목표와 같으면 카운트 1 추가 
            cnt = cnt + 1 
                
    f = depth + cnt
    
    print('F = ' , f)

    return f

def move_blank(start, goal, depth, movement) :
    idx = start.index(' ')
    start[idx], start[movement] = start[movement], start[idx] #swap
    print('blank ', idx, ' --> ', 'blank ', movement)
        
    return start

def A() :
    global visited, start, goal, depth
    if(start == goal) :
        print('정답입니다!')
        return

    blank_idx = start.index(' ')
    tmp_route = []
    F_queue = []

    #처음이면 바로 출력하고 넘김
    if(depth == 0) :
        F_queue.append(F(start, goal, depth))
        visited = blank_idx
        F_queue.pop()
        print_puzzle(start)
        print()
        depth = depth + 1
        A()
        if(start == goal) :
            return
    

    

    #되돌아갈 노드가 없을 때
    elif(depth == 1) :
        #위쪽 이동
        if(blank_idx > 2) :
            start_up = move_blank(start[:], goal, depth, blank_idx-3)
            F_queue.append(F(start_up, goal, depth))
            print('위쪽 이동')
            tmp_route.append([0, start_up]) #위쪽 : 0
            print_puzzle(start_up)
        
        if(blank_idx < 6) :
            start_down = move_blank(start[:], goal, depth, blank_idx+3)
            F_queue.append(F(start_down, goal, depth))
            print('아래쪽 이동')
            tmp_route.append([1, start_down]) #아래쪽 : 1
            print_puzzle(start_down)

        if(blank_idx % 3 != 0) :
            start_left = move_blank(start[:], goal, depth, blank_idx-1)
            F_queue.append(F(start_left, goal, depth))
            print('왼쪽 이동')
            tmp_route.append([2, start_left]) #왼쪽 : 2
            print_puzzle(start_left)

        if(blank_idx % 3 != 2) :
            start_right = move_blank(start[:], goal, depth, blank_idx+1)
            F_queue.append(F(start_right, goal, depth))
            print('오른쪽 이동')
            tmp_route.append([3, start_right]) #오른쪽 : 3
            print_puzzle(start_right)

        

    #왔던 자리로 다시 가면 안되기 때문에 조건 추가
    else :
        if(blank_idx > 2 and blank_idx - 3 != visited) :
            start_up = move_blank(start[:], goal, depth, blank_idx-3)
            F_queue.append(F(start_up, goal, depth))
            print('위쪽 이동')
            tmp_route.append([0, start_up]) #위쪽 : 0
            print_puzzle(start_up)
        
        if(blank_idx < 6 and blank_idx + 3 != visited) :
            start_down = move_blank(start[:], goal, depth, blank_idx+3)
            F_queue.append(F(start_down, goal, depth))
            print('아래쪽 이동')
            tmp_route.append([1, start_down]) #아래쪽 : 1
            print_puzzle(start_down)

        if(blank_idx % 3 != 0 and blank_idx - 1 != visited) :
            start_left = move_blank(start[:], goal, depth, blank_idx-1)
            F_queue.append(F(start_left, goal, depth))
            print('왼쪽 이동')
            tmp_route.append([2, start_left]) #왼쪽 : 2
            print_puzzle(start_left)

        if(blank_idx % 3 != 2 and blank_idx + 1 != visited) :
            start_right = move_blank(start[:], goal, depth, blank_idx+1)
            F_queue.append(F(start_right, goal, depth))
            print('오른쪽 이동')
            tmp_route.append([3, start_right]) #오른쪽 : 3
            print_puzzle(start_right)
        

    print(depth, '번 째 이동에서 F값들 = ', F_queue)
    depth = depth + 1
    if(depth >= 3) :
        visited = start.index(' ')


    print('갈 수 있는 경로 : ', tmp_route)
    next_idx = F_queue.index(min(F_queue))
    start = tmp_route[next_idx][1] #다음 depth에서 연산할 퍼즐 저장
    route.append(start) #최단경로에 있는 퍼즐 저장

    A()




if __name__ == '__main__' :

    #완료되었을 경우 최단경로 출력
    A()
    
    print('ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ')
    print('최단경로 출력')        
    for i in route :
        print_puzzle(i)
    print('ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ')

