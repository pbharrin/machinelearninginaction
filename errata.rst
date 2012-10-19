Listing 2.2 bottom of page 25-page26

A better version of the function file2matrix() is given below. The code in the book will work.

```javascript
def file2matrix(filename):
    fr = open(filename)
    arrayOLines = fr.readlines()
    numberOfLines = len(arrayOLines)            
    returnMat = zeros((numberOfLines,3))       
    classLabelVector = [] 
    index = 0
    for line in arrayOLines:
        line = line.strip()                     
        listFromLine = line.split('\t')         
        returnMat[index,:] = listFromLine[0:3]  
        classLabelVector.append(int(listFromLine[-1]))
        index += 1
    return returnMat,classLabelVector
```

Page 26:

datingDataMat, datingLabels = kNN.file2matrix('datingTestSet.txt') 
should be:
datingDataMat, datingLabels = kNN.file2matrix('datingTestSet2.txt') 
>>> datingLabels[0:20]
[‘didntLike’, ‘smallDoses’,……
should be:
>>> datingLabels[0:20]
[3, 2, 1, 1, 1, 1, 3, 3, 1, 3, 1, 1, 2, 1, 1, 1, 1, 1, 2, 3]
Listing 2.5 page 32

datingDataMat, datingLabels = file2matrix('datingTestSet.txt')
should be:
datingDataMat, datingLabels = file2matrix('datingTestSet2.txt')
Listing 3.3 page 45

bestFeature = I 
should be:
bestFeature = i
page 104 (not a code listing)

|wTx+b|/ ||w||
should be:
|wTA+b|/||w||
Listing 8.4 page 168

The line:
returnMat = zeros((numIt,n)) 
Should be added before the line: 
ws = zeros((n,1)); wsTest = ws.copy(); wsMax = ws.copy()
Listing 9.5 page 195

yHat = mat((m,1))
Should be:
yHat = mat(zeros((m,1)))
