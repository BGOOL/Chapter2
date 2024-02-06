'''
    AUTHOR: Louis-Hendrik Barboutie
    CONTACT: louis.barboutie@gmail.com, loubar-3@student.ltu.se
    
    SUMMARY: The script runs a matplotlib animation to plot out the data received by the RAVEN test stand. The incoming data comes from a csv file generated by the
        serial_reader.py script, which itself receives the data from the SerialComms.cpp script. The interface is manually designed and an animation function
        handles the updating of the animated artists.

    SCRIPT STRUCTURE:

    DIRECTORY STRUCTURE:
        |
        +-- interface.py
        +-- indicatorClass.py
        +-- dataDisplayClass.py
        +-- data.csv

    CHANGELOG: 
        - 06/12/23: Improve handling of graph labelling, make displays work, add rolling average
        - 15/11/23: Added escape key press handling, header comment
        - 08/11/23: Successful implementation of animation with dummy data

    TODO:
        - Add function to automatically save file upon closing
        - read data properly from csv file
        - add message box for software mode, substate and arduino time
        - add display of latest value on graphs
'''

# ================= #
# === LIBRARIES === #
# ================= #

# external
import matplotlib as mpl
import matplotlib.pyplot as plt
import matplotlib.animation as ani
import numpy as np
import csv
import shutil
import datetime

# internal (functions)
# from update import update 
import resources.displayElements as displayElements
from resources.dataDefinitions import graphData, displayData
from resources.indicatorDefinitions import indicatorData
from resources.modeDefinitions import softwareModes, softwareSubstates, modeData
from resources.rollingAverage import rollingAverage



# ================= #
# === VARIABLES === #
# ================= #

nGraphs     = len(graphData)
nDisplays   = len(displayData)
nIndicators = len(indicatorData)



# ======================= #
# === INTERFACE SETUP === #
# ======================= #

# ------------------------ #
# --- SETUP THE PANELS --- #
# ------------------------ #

# select backend and remove toolbar
mpl.use("qtagg")
plt.rcParams['toolbar'] = 'None'


# create interface figure and separate panels for the graphs, displays and indicators
interface = plt.figure(facecolor='silver') 
dataPanel, infoPanel = interface.subfigures(1, 2, width_ratios=[3,1]) 
indicatorPanel, streamPanel = infoPanel.subfigures(2, 1, height_ratios=[3,2])
displayPanel, lightsPanel = indicatorPanel.subfigures(1,2)
modePanel, logPanel = streamPanel.subfigures(1,2)

# adjust background colors of the subfigures
dataPanel.set_facecolor('silver')
lightsPanel.set_facecolor('slategrey') 
displayPanel.set_facecolor('slategrey')
streamPanel.set_facecolor('slategrey')
modePanel.set_facecolor('slategrey')
logPanel.set_facecolor('slategrey')



# -------------------------------------- #
# --- SETUP GRAPHS IN THE DATA PANEL --- #
# -------------------------------------- #

# create subplot layout (4 rows and 2 columns) for the graphs
graphs = dataPanel.subplots(4, 2)

# adjust spacing so that graphs don't overlap and fill out whole panel
dataPanel.subplots_adjust(left=0.05, right=0.99, bottom=0.03, top=0.97, wspace=0.15, hspace=0.4)

# plotting parameters
labelFontSize = 9
tickFontSize  = 7
nDataPoints   = 625 # the covered time span is then about nDataPoints * updateRate = 10000 ms = 10s
linewidth     = 0.75

# add titles and axis labels to the graphs
# set limits and tick parameters
for i in range(4):
    for j in range(2):
        xLabel = '%s [%s]' % (graphData[i+j*4]['xLabel'], graphData[i+j*4]['xUnit'])
        yLabel = '%s [%s]' % (graphData[i+j*4]['yLabel'], graphData[i+j*4]['yUnit'])
        graphs[i,j].set_xlabel(xLabel, fontsize=labelFontSize)
        graphs[i,j].set_ylabel(yLabel, fontsize=labelFontSize)
        graphs[i,j].set_xlim(0, nDataPoints-1)
        graphs[i,j].set_ylim(graphData[i+j*4]['yLowerBound'], graphData[i+j*4]['yUpperBound'])
        graphs[i,j].set_title(graphData[i+j*4]['title'])
        graphs[i,j].tick_params(axis='y', labelsize=tickFontSize)
        graphs[i,j].tick_params(axis='x', which='both', bottom=False, top=False, labelbottom=False) 
        graphs[i,j].axhline(graphData[i+j*4]['warningValue'], color='red', linestyle='--', linewidth=linewidth)
        graphs[i,j].grid() #linestyle='--')

# initialize line artists
dataLines = []
for i in range(4):
    for j in range(2):
        dataLines.append(graphs[i][j].plot([], [], linewidth=linewidth)[0])

# initialize lists for holding the line data
x_data   = [[0] * nDataPoints] * nGraphs
y_data   = [[0] * nDataPoints] * nGraphs
averages = [0] * nGraphs

# initialize line data
for i in range(nGraphs):
    for j in range(nDataPoints):
        x_data[i][j] = j
    dataLines[i].set_data(x_data[i], y_data[i])



# ------------------------------------------ #
# --- ADD INDICATORS TO THE LIGHTS PANEL --- #
# ------------------------------------------ #

# create a column of subplots for holding the indicators
indicators = lightsPanel.subplots(nIndicators, 1)

# adjust spacing so that indicator boxes don't overlap and fill out whole panel
lightsPanel.subplots_adjust(left=0.04, right=0.98, bottom=0.05, top=0.97, wspace=0.15, hspace=0.1)

# create lists for holding indicator objects, labels and states for easy access
indicatorObjects = []
indicatorStates  = [0] * nIndicators
for i in range(nIndicators):
    indicatorObjects.append(displayElements.indicatorLight(indicatorData[i]['title']))
    
# remove plot background and set title and square aspect ratio of the subplots
# add indicator artists
for i in range(nIndicators):
    indicators[i].set_aspect('equal', 'box')
    indicators[i].axis('off')
    for j in range(len(indicatorObjects[i].objects)):
            indicators[i].add_artist(indicatorObjects[i].objects[j])



# ------------------------------------------- #
# --- ADD DATA DISPLAYS TO THE INFO PANEL --- #
# ------------------------------------------- #

# create subplots for the info panel
displays = displayPanel.subplots(nDisplays, 1)

# adjust subplot geometry
# displayPanel.subplots_adjust(left=0.04, right=0.98, bottom=0.05, top=0.97, wspace=0.15, hspace=0.1)

# create display objects and value list
displayObjects = []
displayValues  = [0] * nDisplays
for i in range(nDisplays):
    displayObjects.append(displayElements.valueBox(displayData[i]['title'], displayData[i]['yUnit']))

# remove graph background and set aspect ratio of the displays
# add display artists and format display axes
for i in range(nDisplays):
    displays[i].set_title(displayObjects[i].title, fontsize=displayObjects[i].titleFontSize)
    displays[i].set_xlim(0, 2.5)
    displays[i].set_ylim(0, 1.0)
    displays[i].set_aspect(aspect='equal')
    displays[i].axis('off')
    for j in range(len(displayObjects[i].objects)):
        displays[i].add_artist(displayObjects[i].objects[j])
    

# ----------------------------------- #
# --- ADD SOFTWARE STATE DISPLAYS --- #
# ----------------------------------- #

nModeDisplays = len(modeData)
modeDisplays = modePanel.subplots(nModeDisplays)

for i in range(nModeDisplays):
    displayObjects.append(displayElements.textBox(title=modeData[i], text='TBD'))
    modeDisplays[i].axis('off')
    modeDisplays[i].set_ylim(0, 1.0)
    modeDisplays[i].set_xlim(0, 2.5)
    modeDisplays[i].set_aspect(aspect='equal')
    modeDisplays[i].set_title(displayObjects[nDisplays + i].title, fontsize=displayObjects[nDisplays + i].titleFontSize)
    for j in range(len(displayObjects[nDisplays + i].objects)):
        modeDisplays[i].add_artist(displayObjects[nDisplays + i].objects[j])

# ----------------------- #
# --- ADD LOG DISPLAY --- #
# ----------------------- #

logDisplay = logPanel.subplots(1)
logDisplay.axis('off')
logDisplay.set_aspect(aspect='equal')
logDisplay.set_ylim(0, 1.6)
logDisplay.set_title('Log messages', fontsize=14)

logObject = displayElements.logBox()
for i in range(len(logObject.objects)):
    logDisplay.add_artist(logObject.objects[i])


# =========================== #
# === INTERFACE ANIMATION === #
# =========================== #

# ------------------------------ #
# --- CREATE LIST OF ARTISTS --- #
# ------------------------------ #

graphArtists = dataLines # nGraphs*2 elements

indicatorArtists = []
for i in range(nIndicators):
    indicatorArtists += indicatorObjects[i].objects # nIndicator*5 elements

displayArtists = []
for i in range(nDisplays):
    displayArtists += displayObjects[i].objects # nDisplays*2 elements

# Store amount of artists for ease of access in the update function
nGraphArtists     = len(graphArtists)
nIndicatorArtists = len(indicatorArtists)
nDisplayArtists   = len(displayArtists)

artists = graphArtists + indicatorArtists + displayArtists

lastFilePosition = 0

def update(frame):
    global lastFilePosition
    global indicatorStates
    global displayValues

    # ------------------------------- #
    # --- READ DATA FROM CSV FILE --- #
    # ------------------------------- #

    # indices in csv file coming from serialReadyer.py and serialComms.cpp
    # Total amount of columns : 21
    # Chamber pressure : 5
    # Bottle pressure : 3
    # Line pressure : 4
    # Bottle Temperature : 7
    # Nozzle Temperature : 9
    # Load Cell Force : 6
    # Ambient Temperature : 10
    # Plume Temperature : 11

    # indicator flag indices 
    # heating blanket : 13
    # main valve : 16
    # ignition relay : 14

    # dataIndices = [5, 3, 4, 7, 9, 10, 11]
    # indicatorIndices = [13, 16, 14]
    # with open('data.csv', 'r', newline='') as dataFile:
    #     dataFile.seek(lastFilePosition)
    #     csvReader = csv.reader(dataFile)
    #     for row in csvReader:
    #         for i in range(nGraphs):
    #             y_data[i].pop(0)
    #             y_data[i].append(float(row[dataIndices[i]]))
    #             indicatorStates[i] = float(row[indicatorIndices[i]])
        # lastFilePosition = dataFile.tell()

    with open('dummyData.csv', 'r', newline='') as dataFile:
        dataFile.seek(lastFilePosition)
        csvReader = csv.reader(dataFile)
        for row in csvReader:
            for i in range(nGraphs):
                y_data[i].pop(0)
                y_data[i].append(float(row[i+1]))
        lastFilePosition = dataFile.tell()



    # ---------------------------- #
    # --- UPDATE DATA IN PLOTS --- #
    # ---------------------------- #

    # update the data and average lines and calculate averages
    for i in range(nGraphs):
        averages[i] = rollingAverage(y_data[i])
        artists[i].set_ydata(y_data[i])


    # ----------------------------------------- #
    # --- DUMMY UPDATE THE INDICATOR LIGHTS --- #
    # ----------------------------------------- #

    # dummy cycle indicator list around
    if frame % 10 == 0:
        item = indicatorStates[0]
        indicatorStates.pop(0)
        indicatorStates.append(item)

    # update the indicator state
    for i in range(nIndicators):
        indicatorObjects[i].setState(indicatorStates[i])
    


    # ----------------------------- #
    # --- UPDATE DISPLAY VALUES --- #
    # ----------------------------- #
    
    for i in range(nDisplays):
        displayValues[i] = averages[i] # change this once new indexing going on
        displayObjects[i].setValue(displayValues[i])
        
        artistIndex = nGraphArtists + nIndicatorArtists + 3*i + 2 

        # update color
        displayObjects[i].value = averages[i]
        if averages[i] >= displayData[i]['warningValue']:
            artists[artistIndex].set_color('darkorange')  
        else :
            artists[artistIndex].set_color('green')

    return artists



# toggle fullscreen
#plt.get_current_fig_manager().full_screen_toggle()

# animation settings
isUsingBlit = True
isCachingFrameData = False
updateRate = 16 # milli seconds: seems to be faster than data rate



# function to add escape key press to exit figure
def escape(esc):
    if esc.key == 'escape':
        plt.close()
        
def fullscreen(f):
    if f.key == 'f':
        plt.get_current_fig_manager().full_screen_toggle()

def on_close(event):
    """
        Save a copy of the data file to archive and with timestamp
    """
    time = datetime.datetime.now().strftime('%Y-%m-%d_%Hh%M') 
    src = 'data.csv'
    dst = 'archive/'
    dst = dst + 'data_' + time + '.csv'
    shutil.copy2(src, dst)

# link the events fto the interface
interface.canvas.mpl_connect('key_press_event', escape)
interface.canvas.mpl_connect('close_event', on_close)

# animate the interface
interfaceAnimation = ani.FuncAnimation(interface, update, interval=updateRate, blit=isUsingBlit, cache_frame_data=isCachingFrameData)
plt.show()