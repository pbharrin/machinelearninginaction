from numpy import *
import operator
from os import listdir
import matplotlib
import matplotlib.pyplot as plt

# 1.prepare data
def file2matrix(filename):
    love_dictionary = {'largeDoses': 3, 'smallDoses': 2, 'didntLike': 1}
    fr = open(filename)
    arrayOLines = fr.readlines()
    numberOfLines = len(arrayOLines)            # get the number of lines in the file
    returnMat = zeros((numberOfLines, 3))       # prepare matrix to return
    classLabelVector = []                       # prepare labels return
    index = 0
    for line in arrayOLines:
        line = line.strip()
        listFromLine = line.split('\t')
        returnMat[index, :] = listFromLine[0: -1]
        if(listFromLine[-1].isdigit()):
            classLabelVector.append(int(listFromLine[-1]))
        else:
            classLabelVector.append(love_dictionary.get(listFromLine[-1]))
        index += 1
    return returnMat, classLabelVector


date_mat, class_label = file2matrix('./datingTestSet.txt')


# 2.analysis data
fig = plt.figure()
ax = fig.add_subplot(111)  # split 1*1, and the first zone
ax.scatter(date_mat[:, 1], date_mat[:, 2])
plt.show()
fig = plt.figure()
ax2 = fig.add_subplot(111)  # split 1*1, and the first zone
ax2.scatter(date_mat[:, 1], date_mat[:, 2], 15*array(class_label), 15*array(class_label))
plt.show()
fig = plt.figure()
ax3 = fig.add_subplot(111)  # split 1*1, and the first zone
ax3.scatter(date_mat[:, 0], date_mat[:, 1], 15*array(class_label), 50*array(class_label))
plt.show()


# 2.2 normal data
def autoNorm(dataSet):
    minVals = dataSet.min(0)
    maxVals = dataSet.max(0)
    ranges = maxVals - minVals
    normDataSet = zeros(shape(dataSet))
    m = dataSet.shape[0]
    normDataSet = dataSet - tile(minVals, (m, 1))
    normDataSet = normDataSet/tile(ranges, (m, 1))   # element wise divide
    return normDataSet, ranges, minVals


autoNorm(date_mat)[0: 5][0]

# 3.test classify
def classify0(inX, dataSet, labels, k):
    dataSetSize = dataSet.shape[0]   # get rows
    diffMat = tile(inX, (dataSetSize, 1)) - dataSet
    sqDiffMat = diffMat ** 2
    sqDistances = sqDiffMat.sum(axis=1)
    distances = sqDistances ** 0.5
    sortedDistIndicies = distances.argsort()  # get index
    classCount = {}
    for i in range(k):
        voteIlabel = labels[sortedDistIndicies[i]]
        classCount[voteIlabel] = classCount.get(voteIlabel, 0) + 1  # d.get(key, defalt_v)
    sortedClassCount = sorted(classCount.items(), key=operator.itemgetter(1), reverse=True)
    return sortedClassCount[0][0]


def datingClassTest():
    hoRatio = 0.50      # hold out 10%
    datingDataMat, datingLabels = file2matrix('datingTestSet2.txt')  # load data setfrom file
    normMat, ranges, minVals = autoNorm(datingDataMat)
    m = normMat.shape[0]
    numTestVecs = int(m*hoRatio)
    errorCount = 0.0
    for i in range(numTestVecs):
        classifierResult = classify0(
            normMat[i, :], normMat[numTestVecs:m, :], datingLabels[numTestVecs: m], 5)
        if (classifierResult != datingLabels[i]):
            print(
                "the classifier came back with: %d, the real answer is: %d" %
                 (classifierResult, datingLabels[i]))
            errorCount += 1.0
    print("the total error rate is: %f" % (errorCount/float(numTestVecs)))
    print(errorCount)


datingClassTest()

# predict person
def classifyPerson():
    resultList = ['not at all', 'in small doses', 'in large doses']
    percentTats = float(input("percentage of time spent playing video games?"))
    ffMiles = float(input("frequent flier miles earned per year?"))
    iceCream = float(input("liters of ice cream consumed per year?"))
    datingDataMat, datingLabels = file2matrix('datingTestSet2.txt')
    normMat, ranges, minVals = autoNorm(datingDataMat)
    inArr = array([ffMiles, percentTats, iceCream, ])
    classifierResult = classify0((inArr - minVals)/ranges, normMat, datingLabels, 5)
    print("You will probably like this person: %s" % resultList[classifierResult - 1])


classifyPerson()

# ------- hand writing ------- #
# 1.convert img txt 32*32 into vector 1*1024
def img2vector(filename):
    returnVect = zeros((1, 1024))
    fr = open(filename)
    for i in range(32):
        lineStr = fr.readline()
        for j in range(32):
            returnVect[0, 32*i+j] = int(lineStr[j])
    return returnVect


testVector = img2vector('./testDigits/0_13.txt')
print(testVector[0, 0:63])

# 3.test classify
def handwritingClassTest():
    hwLabels = []
    trainingFileList = os.listdir('./trainingDigits/')
    m = len(trainingFileList)
    trainingMat = zeros((m, 1024))
    for i in range(m):
        fileNameStr = trainingFileList[i]
        fileStr = fileNameStr.split('.')[0]
        classNumStr = int(fileStr.split('_')[0])
        hwLabels.append(classNumStr)
        trainingMat[i, :] = img2vector('./trainingDigits/%s' % fileNameStr)
    testFileList = listdir('./testDigits/')
    errorCount = 0
    mTest = len(testFileList)
    for i in range(mTest):
        fileNameStr = testFileList[i]
        fileStr = fileNameStr.split('.')[0]
        classNumStr = int(fileStr.split('_')[0])
        vectorUnderTest = img2vector('./testDigits/%s' % fileNameStr)
        classifierResult = classify0(vectorUnderTest, trainingMat, hwLabels, 3)
        if (classifierResult != classNumStr):
            print(
                "the classifier came back with: %d, the real answer is: %d" %
                (classifierResult, classNumStr))
            errorCount += 1.0
    print("the total error rate is: %f" % (errorCount/float(mTest)))
    print(errorCount)


handwritingClassTest()


def handwritingClassPredict(fileName):
    vector = img2vector(fileName)
    hwLabels = []
    trainingFileList = os.listdir('./trainingDigits/')
    m = len(trainingFileList)
    trainingMat = zeros((m, 1024))
    for i in range(m):
        fileNameStr = trainingFileList[i]
        fileStr = fileNameStr.split('.')[0]
        classNumStr = int(fileStr.split('_')[0])
        hwLabels.append(classNumStr)
        trainingMat[i, :] = img2vector('./trainingDigits/%s' % fileNameStr)
    classifierResult = classify0(vector, trainingMat, hwLabels, 3)
    return classifierResult


fileName = './testDigits/1_11.txt'
classifierResult = handwritingClassPredict(fileName)
print(classifierResult)
