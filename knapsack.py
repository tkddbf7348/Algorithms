#식에 필요한 전역변수들
n = 5
W = 9
p = [0, 20, 30, 35, 12, 3]
w = [0, 2, 5, 7, 3, 1]
weight = 0
profit = 0
totweight = 0
bestset = [] #해답
include = []
numbest = 0 # 고려한 아이템 수
maxprofit = 0 #최고 수익
maxweight = 0 #최고 무게

def main() :
    knapsack(-1, 0, 0)
    print('수익 : ', maxprofit, '무게 : ', maxweight)
    for i in range(numbest) :
        print(i+1, ' = ', bestset[i], end='   ')

def knapsack(i, profit, weight) : #DFS 배낭채우기
    global n, W, w, p, totweight, maxprofit, maxweight, bestset, include, numbest #전역변수 선언

    print('i = ', i, 'profit = ', profit, 'weight = ', weight, 'maxprofit = ', maxprofit)

    if weight<=W and profit > maxprofit :
        maxprofit = profit
        maxweight = weight
        numbest = i
        bestset = include

    if(promising(i, profit, weight)) :
        include.insert(i+1, 'yes')
        knapsack(i+1, profit + p[i+1], weight + w[i+1])
        include.insert(i+1, 'no')
        knapsack(i + 1, profit, weight)

def promising(i, profit, weight) : #노드 유효성 판단(무게, 값 측정 후 유효마디인지)
    if(weight >= W) :
        return False
    else :
        j = i + 1
        bound = profit #bound = 한계치
        totweight = weight

        while(j <= n and totweight + w[j] <= W) :
            totweight += w[j]
            bound += p[j]
            j += 1

        k = j;
        if(k <= n) :
            bound = bound + (W - totweight) * p[k] / w[k] #한계치 수식


        return bound > maxprofit #한계치가 최종수익보다 높으면 유효함


main()
