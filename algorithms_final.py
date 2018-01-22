import numpy as np
from PIL import Image
import matplotlib.pyplot as plt
import matplotlib.cm as cm
import random
import os

# MAP CONSTRUCTION : RANDOM AND RECURSIVE APPROACH #

def recursive(width,height):
    """
    From one pixel representing a wall and situated in (height,width) coordinates, build recursively walls or obstacles situated around.
    A probability p2 is used to define the expansion probability from one obstacle's pixel.
    """
    if Map[height][width]==0:
        return 0
    else:
        if height>=2 and Map[height-1][width]==0:
            Map[height-1][width] = np.random.binomial(1,p2)
            if Map[height-1][width]==1:
                return recursive(width,height-1)
        if height<=h-2 and Map[height+1][width]==0:
            Map[height+1][width] = np.random.binomial(1,p2)
            if Map[height+1][width]==1:
                return recursive(width,height+1)
        if width>=2 and Map[height][width-1]==0:
            Map[height][width-1] = np.random.binomial(1,p2)
            if Map[height][width-1]==1:
                return recursive(width-1,height)
        if width<=w-2 and Map[height][width+1]==0:
            Map[height][width+1] = np.random.binomial(1,p2)
            if Map[height][width+1]==1:
                return recursive(width+1,height)
        return 0

def build_map_highly_random():
    """
    Building a map with height h (global variable) and width w.
    It uses the recursive process implemented in recursive function.
    """
    # Editing the border
    Map[:,0] = 1
    Map[-1,:] = 1
    Map[:,-1] = 1

    # Choosing some filled pixel randomly with a probability p1
    for i in range(1,h-1):
        for j in range(1,w-1):
            Map[i][j] = np.random.binomial(1,p1)

    # Build group of obstacles, wall around the filled pixels.
    for i in range(h):
        for j in range(1,w):
            if Map[i][j] ==1:
                recursive(j,i)

def rectangle(w1,h1,w2,h2):
    """
    Colorate the rectangle from (h1,w1) to (h2,w2) as a wall.
    """
    Map[h1:h2,w1:w2] = 1

#################################################
    
# MAP CONSTRUCTION : RANDOM BLOCS CREATION #

def build_map_blocs_wall():
    """
    Building a whole map with:
    - probability p1 to get the place of obstacles
    - random size of rectangular obstacle
    """
    # Editing the border
    Map[:,0] = 1
    Map[-1,:] = 1
    Map[:,-1] = 1

    # Choosing some filled pixel randomly with a probability p
    for i in range(9,h-5):
        for j in range(5,w-5):
            if Map[i][j] !=1:
                Map[i][j] = np.random.binomial(1,p)
                if Map[i][j] ==1:
                    rectangle(j,i,j+random.randint(1,10),i+random.randint(1,10))

#################################################
    
# FUNCTION TO DO STATISTICS ABOUT EFFICIENCY OF ALGOS #

def count_undiscovered(matrix):
    """count undiscovered pixels in the map"""
    count = 0
    for i in range(h):
        for j in range(w):
            if matrix[i][j]==0:
                count= count+1
    return count/(h*w)

def plot_undiscovered(name,i):
    """Plot undiscovered pixels rate with cost in axis-x"""
    global costList
    global undiscoveredList
    #print(costList)
    #print(undiscoveredList)
    plt.title('Not discovered = f(Cost)')
    plt.xlabel('cost')
    plt.ylabel('% not discovered')
    plt.plot(costList,undiscoveredList)
    #plt.show()
    plt.savefig("test"+str(i)+"/plot_"+str(name)+".png")
    plt.close()

#################################################
    
# FUNCTION TO SHOW AND SAVE THE MAPS #

def show_table_1(i):
    """
    The function is used to show the real map.
    The input i is just here to select the destination folder.
    """
    data = np.array(255-Map*255)
    img = Image.fromarray(data)
    plt.imshow(img, cmap=cm.gray, vmin=0, vmax=255)
    #plt.show()
    plt.imsave("test"+str(i)+"/map.png",img)
    plt.close()

def show_table_2(name,i):
    """
    The function is used to show the map representing the current knowledge of the robot.
    The input i is just here to select the destination folder.
    """
    data = np.array(Map2)
    img = Image.fromarray(data)
    plt.imshow(img, cmap=cm.gray, vmin=0, vmax=255)
    #plt.show()
    plt.imsave("test"+str(i)+"/map_"+str(name)+".png",img)
    plt.close()

def show_table_final(name,i):
    """
    The function is used to show the map representing the current knowledge of the robot + the theoretical place of the obstacles and the walls.
    The input i is just here to select the destination folder.
    """
    Mapfinal = Map2
    for k in range(h):
        for j in range(w):
            if Map[k][j]==1:
                Mapfinal[k][j]=128     
    data = np.array(Mapfinal)
    img = Image.fromarray(data)
    plt.imshow(img, cmap=cm.gray, vmin=0, vmax=255)
    #plt.show()
    plt.imsave("test"+str(i)+"/final_map_"+str(name)+".png",img)
    plt.close()

#################################################
    
# SOME USEFUL FUNCTIONS #

def colour_start_space():
    """
    The function is used to colour the start space in the map known by the robot
    """
    global globx
    global globy
    Map2[globy-2:globy+3,globx-2:globx+3] = 255

def null(a):
    """
    return 1 if a=0 and 0 else
    """
    if a==0:
        return 1
    else:
        return 0

def addList(L1,L2):
    if len(L1)!=len(L2):
        return L1
    else:
        return [L1[i]+L2[i] for i in range(len(L1))]

#################################################
    
# BASICS MOVING FUNCTIONS #

def can_move_forward():
    """
    Discover teh cells situated just in front of the robot and returns a boolean : True if the robot can move forward..
    """
    global globx
    global globX
    global globy
    global globY
    global TETA
    global directionX
    global directionY
    global cost
    global Map2
    is_possible=True

    # Check if the robot won't go outside the arena
    if (directionY==1 and globy==h-2) or (directionY==-1 and globy==2) or (directionX==1 and globx==w-2) or (directionX==-1 and globx==2) :
        is_possible = False

    # The 5 following statements enable to test the 5 cells situated in front of the robot
    if Map[globy+3*directionY-2*null(directionY),globx+3*directionX-2*null(directionX)]==1:
        is_possible = False 
        Map2[globy+3*directionY-2*null(directionY),globx+3*directionX-2*null(directionX)]=128
    else:
        Map2[globy+3*directionY-2*null(directionY),globx+3*directionX-2*null(directionX)]=255
        
    if Map[globy+3*directionY-null(directionY),globx+3*directionX-null(directionX)]==1:
        is_possible = False 
        Map2[globy+3*directionY-null(directionY),globx+3*directionX-null(directionX)]=128
    else:
        Map2[globy+3*directionY-null(directionY),globx+3*directionX-null(directionX)]=255

    if Map[globy+3*directionY,globx+3*directionX]==1:
        is_possible = False
        Map2[globy+3*directionY,globx+3*directionX]=128
    else:
        Map2[globy+3*directionY,globx+3*directionX]=255

    if Map[globy+3*directionY+null(directionY),globx+3*directionX+null(directionX)]==1:
        is_possible = False
        Map2[globy+3*directionY+null(directionY),globx+3*directionX+null(directionX)]=128
    else:
        Map2[globy+3*directionY+null(directionY),globx+3*directionX+null(directionX)]=255

    if Map[globy+3*directionY+2*null(directionY),globx+3*directionX+2*null(directionX)]==1:
        is_possible = False
        Map2[globy+3*directionY+2*null(directionY),globx+3*directionX+2*null(directionX)]=128
    else:
        Map2[globy+3*directionY+2*null(directionY),globx+3*directionX+2*null(directionX)]=255

    # assess the cost represented by the use of the function
    increase_cost(2)
    
    return is_possible

def disclosed(angle):
    """
    check if the cells linked to the direction corresponding to the angle have already been disclosed
    """
    global globx
    global globX
    global globy
    global globY
    global Map2
    angle = angle%360
    if angle==0:
        dirX = 0
        dirY = 1
    elif angle==90:
        dirX  = 1
        dirY = 0
    elif angle==180:
        dirX =0
        dirY=-1
    else:
        dirX =-1
        dirY=0
        
    if Map2[globy+3*dirY,globx+3*dirX]==0 or Map2[globy+3*dirY+null(dirY),globx+3*dirX+null(dirX)]==0 or Map2[globy+3*dirY+null(dirY),globx+3*dirX+2*null(dirX)]==0 or Map2[globy+3*dirY-null(dirY),globx+3*dirX-null(dirX)]==0 or Map2[globy+3*dirY-null(dirY),globx+3*dirX-2*null(dirX)]==0:
        return False
    else:
        return True

def move_forward():
    """
    The function enables to move the robot 5cm forward if it is possible.
    It returns True if the movement has been possible.
    """
    global globx
    global globX
    global globy
    global globY
    global TETA
    global directionX
    global directionY
    global cost
    if can_move_forward():
        globX=globX+5*directionX
        globY=globY+5*directionY
        globx=int(globX/5)
        globy=int(globY/5)
        increase_cost(1)
        return True
    else:
        return False

def move_forward_until(max_pos):
    """
    The function enables to move the robot forward until max_pos has been reached or the robot cannot move anymore.
    It returns the number of cells the robot has been able to do.
    """
    global globx
    global globX
    global globy
    global globY
    global TETA
    global directionX
    global directionY
    global cost
    i = 0
    while i<max_pos and can_move_forward():
        globX=globX+5*directionX
        globY=globY+5*directionY
        globx=int(globX/5)
        globy=int(globY/5)
        increase_cost(1)
        i=i+1
    return i

def rotate(angle):
    """
    The function enables to rotate the robot of a certain angle.
    """
    global TETA
    global directionX
    global directionY
    global cost
    TETA=(TETA+angle)%360
    if TETA==0:
        directionX = 0
        directionY = 1
    elif TETA==90:
        directionX = 1
        directionY = 0
    elif TETA ==180:
        directionX=0
        directionY=-1
    else:
        directionX=-1
        directionY=0
    increase_cost(1)

#################################################
    
# ADVANCED MOVING FUNCTIONS #

def nearest_undisclosed_free(angle):
    """
    The function returns the biggest number of 5cm-cell with the angle "angle" that the robot can do.
    """
    global globx
    global globX
    global globy
    global globY
    global Map2
    angle=angle%360
    if angle==0:
        dirX = 0
        dirY = 1
    elif angle==90:
        dirX  = 1
        dirY = 0
    elif angle==180:
        dirX =0
        dirY=-1
    else:
        dirX =-1
        dirY=0
        
    i=0
    case1=128
    case2=128
    case3=128
    case4=128
    case5=128
    
    # we check that the cells assessed are inisde the arena
    if globy+(3+i)*dirY+2*null(dirY)<h and globy+(3+i)*dirY-2*null(dirY)>0 and globx+(3+i)*dirX+2*null(dirX)<w and globx+(3+i)*dirX-2*null(dirX)>0:
        case1 = Map2[globy+(3+i)*dirY-2*null(dirY),globx+(3+i)*dirX-2*null(dirX)]
        case2 = Map2[globy+(3+i)*dirY-null(dirY),globx+(3+i)*dirX-null(dirX)]
        case3 = Map2[globy+(3+i)*dirY,globx+(3+i)*dirX]
        case4 = Map2[globy+(3+i)*dirY+null(dirY),globx+(3+i)*dirX+null(dirX)]
        case5 = Map2[globy+(3+i)*dirY+2*null(dirY),globx+(3+i)*dirX+2*null(dirX)]

    # we loop until a wall has been found and an undisclosed place has not been found
    while case1!=128 and case2!=128 and case3!=128 and case4!=128 and case5!=128 and globy+(3+i)*dirY+2*null(dirY)<h and globy+(3+i)*dirY-2*null(dirY)>0 and globx+(3+i)*dirX+2*null(dirX)<w and globx+(3+i)*dirX-2*null(dirX)>0:
        if case1!=0 and case2!=0 and case3!=0 and case4!=0 and case5!=0:
            i=i+1
            if globy+(3+i)*dirY+2*null(dirY)<h and globy+(3+i)*dirY-2*null(dirY)>0 and globx+(3+i)*dirX+2*null(dirX)<w and globx+(3+i)*dirX-2*null(dirX)>0:
                case1 = Map2[globy+(3+i)*dirY-2*null(dirY),globx+(3+i)*dirX-2*null(dirX)]
                case2 = Map2[globy+(3+i)*dirY-null(dirY),globx+(3+i)*dirX-null(dirX)]
                case3 = Map2[globy+(3+i)*dirY,globx+(3+i)*dirX]
                case4 = Map2[globy+(3+i)*dirY+null(dirY),globx+(3+i)*dirX+null(dirX)]
                case5 = Map2[globy+(3+i)*dirY+2*null(dirY),globx+(3+i)*dirX+2*null(dirX)]
        else:
            return i+1
    return 1000

def nearest_undisclosed_point():
    """return the angle the robot should take to go to the nearest undisclosed point in one of the four direction without obstacle between, 1000 if not exists"""
    global TETA
    global globx
    global globX
    global globy
    global globY
    angle=0
    minimum = nearest_undisclosed_free(TETA+angle)
    indexAngle = angle
    while angle<270:
        angle=angle+90
        dist = nearest_undisclosed_free(TETA+angle)
        if dist<minimum:
            minimum=dist
            indexAngle=angle
    return indexAngle,minimum

def increase_cost(value):
    """increase the cost function by the value 'value'. Updating costList and undiscoveredList as well."""
    global cost
    global costList
    global undiscoveredList
    global costPrecision
    if cost%costPrecision>(value+cost)%costPrecision:
        costList.append(int((cost+value)/costPrecision)*costPrecision)
        undiscoveredList.append(count_undiscovered(Map2))
    cost = cost+value
    
#################################################
    
# ALGORITHMS CHENILLE #

def algo_chenille(i):
    global cost
    global threshold
    global costList
    global undiscoveredList
    while cost<=threshold:
        terminated = False
        arg = 0
        while not terminated:
            move_forward_until(max([h,w]))
            rotate(90)
            nb = move_forward_until(5)
            if nb == 0:
                arg = arg+1
            rotate(90)
            move_forward_until(max([h,w]))
            rotate(-90)
            nb = move_forward_until(5)
            if nb == 0:
                arg = arg+1
            rotate(-90)
            if arg==2:
                terminated = True
            arg = 0
        rotate(-90)

#################################################
    
# ALGORITHMS ESCARGOT #

def algo_escargot(i):
    global cost
    global threshold
    global costList
    global undiscoveredList
    rotate(-90)
    while cost<threshold:
        terminated = False
        distanceShort=5
        while not terminated and cost<threshold:
            arg=0
            boolean = move_forward()
            if not boolean:
                arg=arg+1
            rotate(90)
            move_forward_until(distanceShort)
            rotate(90)
            move_forward_until(distanceShort*2)
            rotate(-90)
            move_forward_until(distanceShort)
            rotate(-90)
            boolean = move_forward()
            if not boolean:
                arg=arg+1
            rotate(-90)
            distanceShort=distanceShort+5
            move_forward_until(distanceShort)
            rotate(-90)
            move_forward_until(distanceShort*2)
            rotate(-90)
            move_forward_until(distanceShort)
            rotate(90)
            if arg>=1:
                terminated = True
            arg = 0
            distanceShort=distanceShort+5
        rotate(-90)

#################################################
    
# ALGORITHMS RECURSIVE #

def algo_recursive(i):
    global cost
    global threshold
    global costList
    global undiscoveredList
    while cost<=threshold:
        if not disclosed(TETA+0):
            move_forward()
        else:
            if not disclosed(TETA+90):
                rotate(90)
                move_forward()
            elif not disclosed(TETA-90):
                rotate(-90)
                move_forward()
            else:
                if can_move_forward():
                    move_forward()
                else:
                    rotate(90)
        
#################################################
    
# ALGORITHMS RANDOM #

def algo_random(i):
    global cost
    global threshold
    global costList
    global undiscoveredList
    while cost<=threshold:
        if np.random.binomial(1,0.5)==1:
            if np.random.binomial(1,0.5)==1:
                rotate(90)
            else:
                rotate(-90)
        else:
            if np.random.binomial(1,0.5)==1:
                move_forward()

#################################################
    
# ALGORITHMS RECURSIVE B #

def algo_recursive_b(i):
    global cost
    global threshold
    global costList
    global undiscoveredList
    while cost<=threshold:
        if not disclosed(TETA+0):
            move_forward()
        else:
            if not disclosed(TETA+90):
                rotate(90)
                move_forward()
            elif not disclosed(TETA-90):
                rotate(-90)
                move_forward()
            else:
                indexAngle,minimum = nearest_undisclosed_point()
                if minimum==1000:
                    if can_move_forward():
                        move_forward()
                    else:
                        if np.random.binomial(1,0.5)==1:
                            rotate(90)
                        else:
                            rotate(-90)
                else:
                    rotate(indexAngle)
                    move_forward_until(minimum)

H = 400 # height in cm
W = 200 # width in cm
p = 0.006
costPrecision=1000
threshold = 50000
testsNumber = 100
algosList = [algo_chenille,algo_escargot,algo_recursive,algo_recursive_b,algo_random]
algosListName=["chenille","escargot","recursive","recursive B","random"]
xAxisCostList=[costPrecision*i for i in range(int(threshold/costPrecision)+1)]
yAxisUndiscovered=[[0 for i in range(len(xAxisCostList))] for j in range(len(algosList))]

h = int(H/5)
w = int(W/5)

for i in range(testsNumber):

    Map = np.array([[0 for i in range(w)] for j in range(h)]) # code : 0 = nothing ; 1 = wall
    build_map_blocs_wall()
    if not os.path.exists("test"+str(i)):
        os.makedirs("test"+str(i))
    show_table_1(i)

    for k in range(len(algosList)):
        
        name = algosListName[k]
        cost=0
        globX=40
        globY=10
        globx=int(globX/5)
        globy=int(globY/5)
        TETA = 0
        directionX=0
        directionY=1
        Map2 = np.array([[0 for i in range(w)] for j in range(h)])  # code : 0 = not known ; 255 = free ; 128 = wall
        costList=[0]
        undiscoveredList=[1]

        # Algo
        print("Algo "+name)
        colour_start_space()
        algosList[k](i)
        show_table_2(name,i)
        show_table_final(name,i)
        plot_undiscovered(name,i)
        yAxisUndiscovered[k] = addList(yAxisUndiscovered[k],undiscoveredList)

"""
plt.title('Not discovered = f(Cost)')
plt.xlabel('Cost')
plt.ylabel('% not discovered')
plt.plot(xAxisCostList, np.array(yAxisUndiscovered[0])/testsNumber, 'r', xAxisCostList, np.array(yAxisUndiscovered[1])/testsNumber, 'b', xAxisCostList, np.array(yAxisUndiscovered[2])/testsNumber, 'g',xAxisCostList, np.array(yAxisUndiscovered[3])/testsNumber,'y')
plt.show()
plt.savefig("summary.png")
plt.close()
"""
fig, ax = plt.subplots()
plt.title('Not discovered = f(Cost)')
plt.xlabel('Cost')
plt.ylabel('% not discovered')
for k in range(len(algosList)):
    ax.plot(xAxisCostList, np.array(yAxisUndiscovered[k])/testsNumber, label=algosListName[k])
plt.legend()
plt.savefig("summary.png")
plt.show()
plt.close()
