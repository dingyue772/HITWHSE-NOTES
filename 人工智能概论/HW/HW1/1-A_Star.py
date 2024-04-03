# 使用包
import numpy as np

# State类定义
class State:
    def __init__(self, state, directionFlag=None, parent=None, f=0):
        self.state = state # 代表当前状态的八数码矩阵
        self.direction = ['up', 'down', 'right', 'left']
        if directionFlag: # 标志移动方向，生成子状态时该方向即为不可移动方向
            self.direction.remove(directionFlag)
        self.parent = parent # 当前状态的父状态
        self.f = f # 代价函数的值

    def getDirection(self): # 返回当前位置的可行移动方向列表
        return self.direction 

    def setF(self, f): # 设置代价函数
        self.f = f
        return

    # 打印结果
    def showInfo(self): # 打印当前状态的信息
        for i in range(len(self.state)):
            for j in range(len(self.state)):
                print(self.state[i, j], end='  ')
            print("\n")
        print('->')
        return

    # 获取0点
    def getZeroPos(self): # 获取八数码矩阵中数字0的位置
        postion = np.where(self.state == 0)
        return postion

    # 曼哈顿距离  f = g + h，g=1，如果用宽度优先的评估函数可以不调用该函数
    # 计算当前状态的代价函数值
    def getFunctionValue(self):
        cur_node = self.state.copy()
        fin_node = self.answer.copy()
        dist = 0
        N = len(cur_node)

        for i in range(N):
            for j in range(N):
                if cur_node[i][j] != fin_node[i][j]:
                    index = np.argwhere(fin_node == cur_node[i][j])
                    x = index[0][0]  # 最终x距离
                    y = index[0][1]  # 最终y距离
                    dist += (abs(x - i) + abs(y - j))
        return dist + 1

    def nextStep(self): # 根据当前状态生成下一步可能的状态
        if not self.direction:
            return []
        subStates = []
        boarder = len(self.state) - 1
        # 获取0点位置
        x, y = self.getZeroPos()
        # 向左
        if 'left' in self.direction and y > 0:
            s = self.state.copy()
            tmp = s[x, y - 1]
            s[x, y - 1] = s[x, y]
            s[x, y] = tmp
            news = State(s, directionFlag='right', parent=self)
            news.setF(news.getFunctionValue())
            subStates.append(news)
        # 向上
        if 'up' in self.direction and x > 0:
            # it can move to upper place
            s = self.state.copy()
            tmp = s[x - 1, y]
            s[x - 1, y] = s[x, y]
            s[x, y] = tmp
            news = State(s, directionFlag='down', parent=self)
            news.setF(news.getFunctionValue())
            subStates.append(news)
        # 向下
        if 'down' in self.direction and x < boarder:
            # it can move to down place
            s = self.state.copy()
            tmp = s[x + 1, y]
            s[x + 1, y] = s[x, y]
            s[x, y] = tmp
            news = State(s, directionFlag='up', parent=self)
            news.setF(news.getFunctionValue())
            subStates.append(news)
        # 向右
        if self.direction.count('right') and y < boarder:
            # it can move to right place
            s = self.state.copy()
            tmp = s[x, y + 1]
            s[x, y + 1] = s[x, y]
            s[x, y] = tmp
            news = State(s, directionFlag='left', parent=self)
            news.setF(news.getFunctionValue())
            subStates.append(news)
        # 返回F值最小的下一个点
        # subStates.sort(key=compareNum)
        return subStates

    # A* 迭代
    def solve(self):
        # openList
        openTable = []
        # closeList
        closeTable = []
        openTable.append(self)
        while len(openTable) > 0:
            # openTable打印
            print("-------------------------openTable---------------------------")
            for s in openTable:
                print(s.state)
            print("-------------------------------------------------------------")

            # closeTable打印
            # print("-------------------------closeTable---------------------------")
            # for s in closeTable:
            #     print(s.state)
            # print("-------------------------------------------------------------")
            # 下一步的点移除open
            n = openTable.pop(0)
            # 加入close
            closeTable.append(n)
            # 确定下一步点
            subStates = n.nextStep()
            openTable.extend(subStates)
            openTable.sort(key=compareNum)
            subState = openTable[0]
            path = []
            # 判断是否和最终结果相同
            if (subState.state == subState.answer).all():
                while subState.parent:
                    path.append(subState.parent)
                    subState = subState.parent
                path.reverse()
                return path
        else:
            return None, None
    def getOriginState(self):
        originState.showInfo()

def compareNum(state):
    return state.f

originState = State(np.array([[2, 8, 3], [1, 6, 4], [7, 0, 5]]))
State.answer = np.array([[1, 2, 3], [8, 0, 4], [7, 6, 5]]) # 用类变量保存八数码问题的目标状态

s1 = State(state=originState.state) 
path = s1.solve()

if path:
    for node in path:
        node.showInfo()
    print(State.answer)
    print("Total steps is %d" % len(path))