'''
Created on Jun 14, 2011
FP-Growth FP means frequent pattern
the FP-Growth algorithm needs: 
1. FP-tree (class treeNode)
2. header table (use dict)

This finds frequent itemsets similar to apriori but does not 
find association rules.  

@author: Peter
'''
class treeNode:
    def __init__(self, nameValue, numOccur):
        self.name = nameValue
        self.count = numOccur
        self.nodeLink = None
        self.children = {} 
    
    def inc(self, numOccur):
        self.count += numOccur
        
    def disp(self, ind):
        print '  '*ind, self.name, ' ', self.count
        for child in self.children.values():
            child.disp(ind+1)

def updateTree(items, inTree, headerTable, count):
    if items[0] in inTree.children:#check if orderedItems[0] in retTree.children
        inTree.children[items[0]].inc(count) #incrament count
    else:   #add items[0] to inTree.children
        inTree.children[items[0]] = treeNode(items[0], count)
        if headerTable[items[0]][1] == None: #update header table 
            headerTable[items[0]][1] = inTree.children[items[0]]
        else:
            updateHeader(headerTable[items[0]][1], inTree.children[items[0]])
    if len(items) > 1:#call updateTree() with remaining ordered items
        updateTree(items[1::], inTree.children[items[0]], headerTable, count)
        
def updateHeader(nodeToTest, targetNode):
    if nodeToTest.nodeLink == None:
        nodeToTest.nodeLink = targetNode
    else:
        updateHeader(nodeToTest.nodeLink, targetNode)
        
def createTree(dataSet, minSup=1): #create FP-tree from dataset but don't mine
    headerTable = {}
    #go over dataSet twice
    for trans in dataSet:#first pass counts frequency of occurance
        for item in trans:
            headerTable[item] = headerTable.get(item, 0) + dataSet[trans]
    for k in headerTable.keys():  #remove items not meeting minSup
        if headerTable[k] < minSup: 
            del(headerTable[k])
    freqItemSet = set(headerTable.keys())
    print 'freqItemSet: ',freqItemSet
    if len(freqItemSet) == 0: return None, None  #if no items meet min support -->get out
    for k in headerTable:
        headerTable[k] = [headerTable[k], None] #reformat headerTable to use Node link 
    print 'headerTable: ',headerTable
    retTree = treeNode('Null Set', 1) #create tree
    for tranSet, count in dataSet.items():  #go through dataset 2nd time
        localD = {}
        for item in tranSet:  #put transaction items in order
            if item in freqItemSet:
                localD[item] = headerTable[item][0]
        orderedItems = [v[0] for v in sorted(localD.items(), key=lambda p: p[1], reverse=True)]
        retTree.disp(1)
        updateTree(orderedItems, retTree, headerTable, count)#populate tree with ordered freq itemset
        retTree.disp(1)
    return retTree, headerTable #return tree and header table
    
def findPrefixPath(basePat, treeNode, prefixPath, condPats):
    prefixPath.append(treeNode.name)
    if (basePat in treeNode.children) and (len(prefixPath) > 1): 
        #print prefixPath
        count = treeNode.children[basePat].count#pull up treeNode.children[basePat] and get count
        key = frozenset(prefixPath[1:])#make frozen set of prefixPath[1:] and use that as dict key
        condPats[key] = count
    for child in treeNode.children.values():
        findPrefixPath(basePat, child, prefixPath, condPats)
    del(prefixPath[-1])
        

def mineTree(inTree, headerTable, minSup, preFix):
    bigL = [v[0] for v in sorted(headerTable.items(), key=lambda p: p[1])]#(sort header table)
    print 'bigL: ',bigL
    #start from bottom of header table
    for basePat in bigL:
        print 'finalFrequent Item: %s%s' % (preFix, basePat)#append to list
        condPattBases = {}
        findPrefixPath(basePat, inTree, [], condPattBases) #1. construct cond pattern base, 
        print 'condPattBases :',basePat, condPattBases
        #2. construct cond FP-tree from cond. pattern base
        myCondTree, myHead = createTree(condPattBases, minSup)
        print 'head from conditional tree: ', myHead
        if myHead != None: 
            myCondTree.disp(1)            #3. mine cond. FP-tree
            mineTree(myCondTree, myHead, minSup, '%s%s' % (preFix,basePat))

simpDat = [['f', 'a', 'c', 'd', 'g', 'i', 'm', 'p'],
           ['a', 'b', 'c', 'f', 'l', 'm', 'o'],
           ['b', 'f', 'h', 'j', 'o'],
           ['b', 'c', 'k', 's', 'p'],
           ['a', 'f', 'c', 'e', 'l', 'p', 'm', 'n'],
           ['f']]

def createInitSet(dataSet):
    retDict = {}
    for trans in dataSet:
        retDict[frozenset(trans)] = 1
    return retDict

minSup = 3
initSet = createInitSet(simpDat)
myFPtree, myHeaderTab = createTree(initSet, minSup)
myFPtree.disp(1)
mineTree(myFPtree, myHeaderTab, minSup, '')
