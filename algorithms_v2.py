import numpy as np
from PIL import Image
import matplotlib.pyplot as plt
import matplotlib.cm as cm
import random
import os

def recursive(width,height):
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

def show_table_1(i):
    data = np.array(255-Map*255)
    img = Image.fromarray(data)
    plt.imshow(img, cmap=cm.gray, vmin=0, vmax=255)
    #plt.show()
    plt.imsave("test"+str(i)+"/map.png",img)
    plt.close()

def show_table_2(name,i):
    data = np.array(Map2)
    img = Image.fromarray(data)
    plt.imshow(img, cmap=cm.gray, vmin=0, vmax=255)
    #plt.show()
    plt.imsave("test"+str(i)+"/map_"+str(name)+".png",img)
    plt.close()

def show_table_final(name,i):
    Mapfinal = Map2
    for k in range(h):
        for j in range(w):
            if Map[k][j]==1:
                Mapfinal[k][j]=128     
    data = np.array(Mapfinal)
    img = Image.fromarray(data)
    plt.imshow(img, cmap=cm.gray, vmin=0, vmax=255)
    plt.show()
    #plt.imsave("test"+str(i)+"/final_map_"+str(name)+".png",img)
    plt.close()

def rectangle(w1,h1,w2,h2):
    Map[h1:h2,w1:w2] = 1

def build_map_highly_random():
    # Editing the border
    Map[:,0] = 1
    Map[-1,:] = 1
    Map[:,-1] = 1

    # Choosing some filled pixel randomly with a probability p1
    for i in range(1,h-1):
        for j in range(1,w-1):
            Map[i][j] = np.random.binomial(1,p1)

    for i in range(h):
        for j in range(1,w):
            if Map[i][j] ==1:
                recursive(j,i)

def build_map_blocs():
    # Editing the border
    Map[:,0] = 1
    Map[-1,:] = 1
    Map[:,-1] = 1

    # Choosing some filled pixel randomly with a probability p1
    for i in range(9,h-5):
        for j in range(1,w-5):
            if Map[i][j] !=1:
                Map[i][j] = np.random.binomial(1,p1bis)
                if Map[i][j] ==1:
                    rectangle(j,i,j+random.randint(1,10),i+random.randint(1,10))

def build_map_blocs_wall():
    # Editing the border
    Map[:,0] = 1
    Map[-1,:] = 1
    Map[:,-1] = 1

    # Choosing some filled pixel randomly with a probability p1
    for i in range(9,h-5):
        for j in range(5,w-5):
            if Map[i][j] !=1:
                Map[i][j] = np.random.binomial(1,p1bis)
                if Map[i][j] ==1:
                    rectangle(j,i,j+random.randint(1,10),i+random.randint(1,10))

def colour_start_space():
    global globx
    global globy
    Map2[globy-2:globy+2,globx-2:globx+2] = 255

def null(a):
    if a==0:
        return 1
    else:
        return 0

def can_move_forward():
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
    #print("Assess")
    #print("("+str(globy+2*directionY)+","+str(globx+2*directionX)+") : "+str(Map2[globy+2*directionY,globx+2*directionX]))
    #print("("+str(globy+2*directionY+null(directionY))+","+str(globx+2*directionX+null(directionX))+") : "+str(Map2[globy+2*directionY+null(directionY),globx+2*directionX+null(directionX)]))
    #print("("+str(globy+2*directionY-null(directionY))+","+str(globx+2*directionX-null(directionX))+") : "+str(Map2[globy+2*directionY-null(directionY),globx+2*directionX-null(directionX)]))
    if (directionY==1 and globy==h-2) or (directionY==-1 and globy==1) or (directionX==1 and globx==w-2) or (directionX==-1 and globx==1) :
        is_possible = False
    indexX = globx+2*directionX-2*null(directionX)*directionY
    indexY = globy+2*directionY+2*null(directionY)*directionX
    if Map[indexY,indexX]==1:
        is_possible = False 
        Map2[indexY,indexX]=128
    else:
        Map2[indexY,indexX]=255

    indexX = globx+2*directionX-null(directionX)*directionY
    indexY = globy+2*directionY+null(directionY)*directionX
    if Map[indexY,indexX]==1:
        is_possible = False 
        Map2[indexY,indexX]=128
    else:
        Map2[indexY,indexX]=255

    indexX = globx+2*directionX
    indexY = globy+2*directionY
    if Map[indexY,indexX]==1:
        is_possible = False 
        Map2[indexY,indexX]=128
    else:
        Map2[indexY,indexX]=255

    indexX = globx+2*directionX+null(directionX)*directionY
    indexY = globy+2*directionY-null(directionY)*directionX
    if Map[indexY,indexX]==1:
        is_possible = False 
        Map2[indexY,indexX]=128
    else:
        Map2[indexY,indexX]=255
    cost = cost+2
    return is_possible

def disclosed(angle):
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
    #print("Test Disclosed")
    #print("("+str(globy+2*dirY)+","+str(globx+2*dirX)+") : "+str(Map2[globy+2*dirY,globx+2*dirX]))
    #print("("+str(globy+2*dirY+null(dirY))+","+str(globx+2*dirX+null(dirX))+") : "+str(Map2[globy+2*dirY+null(dirY),globx+2*dirX+null(dirX)]))
    #print("("+str(globy+2*dirY-null(dirY))+","+str(globx+2*dirX-null(dirX))+") : "+str(Map2[globy+2*dirY-null(dirY),globx+2*dirX-null(dirX)]))
    #print("I'm in ("+str(globy)+","+str(globx)+")")
    #print("Look angle : "+str(angle))
    #print("read("+str(globy+2*dirY)+","+str(globx+2*dirX)+")")
    #print("read("+str(globy+2*dirY+2*null(dirY)*dirX)+","+str(globx+2*dirX-2*null(dirX)*dirY)+")")
    #print("read("+str(globy+2*dirY+null(dirY)*dirX)+","+str(globx+2*dirX-null(dirX)*dirY)+")")
    #print("read("+str(globy+2*dirY-null(dirY)*dirX)+","+str(globx+2*dirX+null(dirX)*dirY)+")")
    if Map2[globy+2*dirY,globx+2*dirX]==0 or Map2[globy+2*dirY+2*null(dirY)*dirX,globx+2*dirX-2*null(dirX)*dirY]==0 or Map2[globy+2*dirY+null(dirY)*dirX,globx+2*dirX-null(dirX)*dirY]==0 or Map2[globy+2*dirY-null(dirY)*dirX,globx+2*dirX+null(dirX)*dirY]==0:
        return False
    else:
        return True

def move_forward():
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
        cost = cost+1
        return True
    else:
        return False

def move_forward_until(max_pos):
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
        cost = cost+1
        i=i+1
    return i

def rotate(angle):
    global globx
    global globX
    global globy
    global globY
    global TETA
    global directionX
    global directionY
    global cost
    angle=angle%360
    if angle==90:
        globX=globX-5*directionX
        globY=globY-5*directionY
    elif angle==180:
        if directionY==1:
            globX=globX-5
            globY=globY-5
        elif directionY==-1:
            globX=globX+5
            globY=globY+5
        elif directionX==1:
            globX=globX-5
            globY=globY+5
        else:
            globX=globX+5
            globY=globY-5
    elif angle==270:
        if directionY==1:
            globX=globX-5
        elif directionY==-1:
            globX=globX+5
        elif directionX==1:
            globY=globY+5
        else:
            globY=globY-5
    globx=int(globX/5)
    globy=int(globY/5)
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
    cost = cost+1

def count_undiscovered(matrix):
    count = 0
    for i in range(h):
        for j in range(w):
            if matrix[i][j]==0:
                count= count+1
    return count/(h*w)

def plot_undiscovered(name,i):
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

def algo_chenille(i):
    global cost
    global threshold
    global costList
    global undiscoveredList
    chenilleNb = 1
    while cost<threshold:
        terminated = False
        arg = 0
        while not terminated:
            move_forward_until(max([h,w]))
            rotate(90)
            nb = move_forward_until(4)
            if nb == 0:
                arg = arg+1
            rotate(90)
            move_forward_until(max([h,w]))
            rotate(-90)
            nb = move_forward_until(4)
            if nb == 0:
                arg = arg+1
            rotate(-90)
            if arg==2:
                terminated = True
            arg = 0
        rotate(-90)
        chenilleNb = chenilleNb+1
        costList.append(cost)
        undiscoveredList.append(count_undiscovered(Map2))
    print("Number of moves : "+str(chenilleNb))

def algo_escargot(i):
    global cost
    global threshold
    global costList
    global undiscoveredList
    chenilleNb = 1
    rotate(-90)
    while cost<threshold:
        terminated = False
        distanceShort=4
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
            distanceShort=distanceShort+4
            move_forward_until(distanceShort)
            rotate(-90)
            move_forward_until(distanceShort*2)
            rotate(-90)
            move_forward_until(distanceShort)
            rotate(90)
            if arg>=1:
                terminated = True
            arg = 0
            distanceShort=distanceShort+4
        rotate(-90)
        chenilleNb = chenilleNb+1
        costList.append(cost)
        undiscoveredList.append(count_undiscovered(Map2))
    print("Number of moves : "+str(chenilleNb))

def will_be_in(xmin,ymin,xmax,ymax):
    global directionX
    global directionY
    global globx
    global globy
    if globx+directionX>=xmin and globx+directionX<=xmax and globy+directionY>=ymin and globy+directionY<=ymax:
        return True
    else:
        return False
    
def move_forward_into_square_limited(imax,xmin,ymin,xmax,ymax):
    global globx
    global globX
    global globy
    global globY
    global TETA
    global directionX
    global directionY
    global cost
    i=0
    boolean = can_move_forward()
    boolean2 = will_be_in(xmin,ymin,xmax,ymax) 
    while boolean2 and boolean and i<imax:
        globX=globX+5*directionX
        globY=globY+5*directionY
        globx=int(globX/5)
        globy=int(globY/5)
        cost = cost+1
        boolean = can_move_forward()
        boolean2 = will_be_in(xmin,ymin,xmax,ymax)
        i=i+1
    # if not boolean2 and boolean: add a door
    if not boolean2:
        return True # we managed to go as far as we could
    else:
        return False

def move_forward_into_square_until(xmin,ymin,xmax,ymax):
    global globx
    global globX
    global globy
    global globY
    global TETA
    global directionX
    global directionY
    global cost
    boolean = can_move_forward()
    boolean2 = will_be_in(xmin,ymin,xmax,ymax) 
    while boolean2 and boolean:
        globX=globX+5*directionX
        globY=globY+5*directionY
        globx=int(globX/5)
        globy=int(globY/5)
        cost = cost+1
        boolean = can_move_forward()
        boolean2 = will_be_in(xmin,ymin,xmax,ymax)
    # if not boolean2 and boolean: add a door
    if not boolean2:
        return True # we managed to go as far as we could
    else:
        return False

#def addDoor(Door,x,y):
 #   if [x,y] not in 

def discover_cell_chenille(factor,xmin,ymin,xmax,ymax):
    rotate(90)
    move_forward_into_square_until(xmin,ymin,xmax,ymax)
    rotate(-90)
    iteration = int(factor/4)
    i=0
    while i<iteration:
        move_forward_into_square_until(xmin,ymin,xmax,ymax)
        rotate(-90)
        move_forward_into_square_limited(2,xmin,ymin,xmax,ymax)
        rotate(-90)
        move_forward_into_square_until(xmin,ymin,xmax,ymax)
        rotate(90)
        move_forward_into_square_limited(2,xmin,ymin,xmax,ymax)
        rotate(90)
        i=i+1
    rotate(-90)
    i=0
    while i<iteration:
        move_forward_into_square_until(xmin,ymin,xmax,ymax)
        rotate(-90)
        move_forward_into_square_limited(2,xmin,ymin,xmax,ymax)
        rotate(-90)
        move_forward_into_square_until(xmin,ymin,xmax,ymax)
        rotate(90)
        move_forward_into_square_limited(2,xmin,ymin,xmax,ymax)
        rotate(90)
        i=i+1

def discover_cell_spirale(factor,xmin,ymin,xmax,ymax):
    rotate(90)
    move_forward_into_square_until(xmin,ymin,xmax,ymax)
    rotate(-90)
    minusLength = int(factor/4)
    length=factor
    while length>=minusLength:
        move_forward_into_square_limited(length,xmin,ymin,xmax,ymax)
        rotate(-90)
        move_forward_into_square_limited(length,xmin,ymin,xmax,ymax)
        rotate(-90)
        move_forward_into_square_limited(length,xmin,ymin,xmax,ymax)
        rotate(-90)
        length=length-minusLength
        move_forward_into_square_limited(length,xmin,ymin,xmax,ymax)

def algo_grid(i):
    global cost
    global threshold
    global costList
    global undiscoveredList
    factor=12
    Cells = [[0 for i in range(int(w/factor))] for j in range(int(h/factor))]
    rotate(-90)
    move_forward_until(w)
    rotate(90)
    move_forward_until(h)
    rotate(90)
    discover_cell_spirale(factor,int(globx/factor)*factor,int(globy/factor)*factor,int(globx/factor)*factor+factor-1,int(globy/factor)*factor+factor-1)
    costList.append(cost)
    undiscoveredList.append(count_undiscovered(Map2))

def algo_recursive(i):
    global cost
    global threshold
    global costList
    global undiscoveredList
    i=0
    while cost<threshold:
        #print("("+str(globx)+","+str(globy)+","+str(TETA)+")")
        #print("(Ici)")
        if not disclosed(TETA+0):
            move_forward()
            #print("this")
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
        i=i+1
        if i%1000==0:
            costList.append(cost)
            undiscoveredList.append(count_undiscovered(Map2))

def nearest_undisclosed_free(angle):
    global globx
    global globX
    global globy
    global globY
    global Map2
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
    #print("Test Disclosed")
    #print("("+str(globy+2*dirY)+","+str(globx+2*dirX)+") : "+str(Map2[globy+2*dirY,globx+2*dirX]))
    #print("("+str(globy+2*dirY+null(dirY))+","+str(globx+2*dirX+null(dirX))+") : "+str(Map2[globy+2*dirY+null(dirY),globx+2*dirX+null(dirX)]))
    #print("("+str(globy+2*dirY-null(dirY))+","+str(globx+2*dirX-null(dirX))+") : "+str(Map2[globy+2*dirY-null(dirY),globx+2*dirX-null(dirX)]))
    i=0
    case1=128
    case2=128
    case3=128
    case4=128
    """
    indexX = globx+2*directionX-2*null(directionX)*directionY
    indexY = globy+2*directionY+2*null(directionY)*directionX
    indexX = globx+2*directionX-null(directionX)*directionY
    indexY = globy+2*directionY+null(directionY)*directionX
    indexX = globx+2*directionX
    indexY = globy+2*directionY
    indexX = globx+2*directionX+null(directionX)*directionY
    indexY = globy+2*directionY-null(directionY)*directionX
    """
    if globy+(2+i)*dirY+null(dirY)<h and globy+(2+i)*dirY-2*null(dirY)>0 and globx+(2+i)*dirX+null(dirX)<w and globx+(2+i)*dirX-2*null(dirX)>0:
        case1 = Map2[globy+(2+i)*dirY,globx+(2+i)*dirX]
        case2 = Map2[globy+(2+i)*dirY-null(dirY)*dirX,globx+(2+i)*dirX+null(dirX)*dirY]
        case3 = Map2[globy+(2+i)*dirY+null(dirY)*dirX,globx+(2+i)*dirX-null(dirX)*dirY]
        case4 = Map2[globy+(2+i)*dirY+2*null(dirY)*dirX,globx+(2+i)*dirX-2*null(dirX)*dirY]
    while case1!=128 and case2!=128 and case3!=128 and case4!=128 and globy+(2+i)*dirY<h and globy+(2+i)*dirY>0 and globx+(2+i)*dirX<w and globx+(2+i)*dirX>0:
        if case1!=0 and case2!=0 and case3!=0 and case4!=0:
            i=i+1
            if globy+(2+i)*dirY<h and globy+(2+i)*dirY>0 and globx+(2+i)*dirX<w and globx+(2+i)*dirX>0:
                case1 = Map2[globy+(2+i)*dirY,globx+(2+i)*dirX]
                case2 = Map2[globy+(2+i)*dirY-null(dirY)*dirX,globx+(2+i)*dirX+null(dirX)*dirY]
                case3 = Map2[globy+(2+i)*dirY+null(dirY)*dirX,globx+(2+i)*dirX-null(dirX)*dirY]
                case4 = Map2[globy+(2+i)*dirY+2*null(dirY)*dirX,globx+(2+i)*dirX-2*null(dirX)*dirY]
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
    

def algo_recursive_b(i):
    global cost
    global threshold
    global costList
    global undiscoveredList
    i=0
    while cost<threshold:
        #print("("+str(globx)+","+str(globy)+","+str(TETA)+")")
        #print("(Ici)")
        #print("check 0")
        if not disclosed(TETA+0):
            move_forward()
            #print("this")
        else:
            """
            TO CORRECT
            """
            #print("check "+str(TETA+90)+" or "+str(TETA-90))
            if not disclosed(TETA+90):
                rotate(90)
                move_forward()
            elif not disclosed(TETA-90):
                rotate(-90)
                move_forward()
            else:
                #print("check for nearest undisclosed point")
                indexAngle,minimum = nearest_undisclosed_point()
                #print("in this case")
                #print("indexAngle : "+str(indexAngle))
                #print("minimum : "+str(minimum))
                if minimum==1000:
                    #print("nothing possible")
                    if can_move_forward():
                        move_forward()
                    else:
                        rotate(90)
                else:
                    #print("i can do stg")
                    rotate(indexAngle)
                    move_forward_until(minimum)
        i=i+1
        if i%10==0:
            show_table_final("recursiveB",i)
        if i%1000==0:
            costList.append(cost)
            undiscoveredList.append(count_undiscovered(Map2))
    costList.append(cost)
    undiscoveredList.append(count_undiscovered(Map2))

H = 400 # height in cm
W = 200 # width in cm
p1 = 0.01
p1bis = 0.005
p2 = 0.3

cost = 0
threshold = 50000
globX=40
globY=10
globx=int(globX/5)
globy=int(globY/5)
TETA = 0
directionX=0
directionY=1
costList=[0]
undiscoveredList=[1]

h = int(H/5)
w = int(W/5)

for i in range(20):
    cost=0
    globX=40
    globY=10
    globx=int(globX/5)
    globy=int(globY/5)
    TETA = 0
    directionX=0
    directionY=1
    Map = np.array([[0 for i in range(w)] for j in range(h)]) # code : 0 = nothing ; 1 = wall
    Map2 = np.array([[0 for i in range(w)] for j in range(h)])  # code : 0 = not known ; 255 = free ; 128 = wall
    costList=[0]
    undiscoveredList=[1]

    build_map_blocs_wall()
    print(Map)
    if not os.path.exists("test"+str(i)):
        os.makedirs("test"+str(i))
    show_table_1(i)
    
    # Chenille
    print("Algo chenille")
    colour_start_space()
    algo_chenille(i)
    show_table_2("chenille",i)
    show_table_final("chenille",i)
    print(costList)
    print(undiscoveredList)
    plot_undiscovered("chenille",i)
    undiscovered = count_undiscovered(Map2)
    print("Not discovered : "+str(undiscovered))

    cost=0
    globX=40
    globY=10
    globx=int(globX/5)
    globy=int(globY/5)
    TETA = 0
    directionX=0
    directionY=1
    costList=[0]
    undiscoveredList=[1]
    Map2 = np.array([[0 for i in range(w)] for j in range(h)])  # code : 0 = not known ; 255 = free ; 128 = wall
    colour_start_space()
    
    # Escargot
    print("Algo escargot")
    colour_start_space()
    algo_escargot(i)
    show_table_2("escargot",i)
    show_table_final("escargot",i)
    print(costList)
    print(undiscoveredList)
    plot_undiscovered("escargot",i)
    undiscovered = count_undiscovered(Map2)
    print("Not discovered : "+str(undiscovered))
    
    cost=0
    globX=40
    globY=10
    globx=int(globX/5)
    globy=int(globY/5)
    TETA = 0
    directionX=0
    directionY=1
    costList=[0]
    undiscoveredList=[1]
    Map2 = np.array([[0 for i in range(w)] for j in range(h)])  # code : 0 = not known ; 255 = free ; 128 = wall
    colour_start_space()
    
    # Recursive
    print("Algo recursive")
    colour_start_space()
    algo_recursive(i)
    show_table_2("recursive",i)
    show_table_final("recursive",i)
    #print(costList)
    #print(undiscoveredList)
    plot_undiscovered("recursive",i)
    undiscovered = count_undiscovered(Map2)
    print("Not discovered : "+str(undiscovered))
    

    cost=0
    globX=40
    globY=10
    globx=int(globX/5)
    globy=int(globY/5)
    TETA = 0
    directionX=0
    directionY=1
    costList=[0]
    undiscoveredList=[1]
    Map2 = np.array([[0 for i in range(w)] for j in range(h)])  # code : 0 = not known ; 255 = free ; 128 = wall
    colour_start_space()

    # Recursive B
    print("Algo recursive B")
    colour_start_space()
    algo_recursive_b(i)
    show_table_2("recursiveB",i)
    show_table_final("recursiveB",i)
    #print(costList)
    #print(undiscoveredList)
    plot_undiscovered("recursiveB",i)
    undiscovered = count_undiscovered(Map2)
    print("Not discovered : "+str(undiscovered))
