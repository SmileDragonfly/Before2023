# A dictionary of movie critics and thheir ratings of a small
# set of movie
critics={'Lisa Rose': {'Lady in the Water': 2.5, 'Snakes on a Plane': 3.5,
'Just My Luck': 3.0, 'Superman Returns': 3.5, 'You, Me and Dupree': 2.5,
'The Night Listener': 3.0},
'Gene Seymour': {'Lady in the Water': 3.0, 'Snakes on a Plane': 3.5,
'Just My Luck': 1.5, 'Superman Returns': 5.0, 'The Night Listener': 3.0,
'You, Me and Dupree': 3.5},
'Michael Phillips': {'Lady in the Water': 2.5, 'Snakes on a Plane': 3.0,
'Superman Returns': 3.5, 'The Night Listener': 4.0},
'Claudia Puig': {'Snakes on a Plane': 3.5, 'Just My Luck': 3.0,
'The Night Listener': 4.5, 'Superman Returns': 4.0,
'You, Me and Dupree': 2.5},
'Mick LaSalle': {'Lady in the Water': 3.0, 'Snakes on a Plane': 4.0,
'Just My Luck': 2.0, 'Superman Returns': 3.0, 'The Night Listener': 3.0,
'You, Me and Dupree': 2.0},
'Jack Matthews': {'Lady in the Water': 3.0, 'Snakes on a Plane': 4.0,
'The Night Listener': 3.0, 'Superman Returns': 5.0, 'You, Me and Dupree': 3.5},
'Toby': {'Snakes on a Plane':4.5,'You, Me and Dupree':1.0,'Superman Returns':4.0}}

from math import sqrt
# Return a distance based similarity score for person 1 and person 2
def sim_distance(prefs,person1,person2):
 #Get the list of shared_items
 si={}
 for item in prefs[person1]:
  if item in prefs[person2]:
   si[item]=1
 #if they have no ratings in common, return 0
 if len(si)==0: 
  return 0
 #Add up the squares of all the differences
 sum_of_squares=sum(pow(prefs[person1][item]-prefs[person2][item],2) 
                     for item in prefs[person1] if item in prefs[person2])
 return 1/(1+sum_of_squares)
 
 
 
# print (sim_distance(critics,'Lisa Rose','Gene Seymour'))

# *******************19/12/2018****************************
# Return the pearson correlation coefficient for p1 and p2
def sim_pearson(prefs,p1,p2):
 # Get the list of mutually rated item
    si={}
    for item in prefs[p1]:
        if item in prefs[p2]:
            si[item]=1
    # Find the number of elements
    n=len(si)
 
    # if they are no rating in common, return 0
    if n==0: return 0
 
    # Add up all the preferences
    sum1=sum(prefs[p1][item] for item in si)
    sum2=sum(prefs[p2][item] for item in si)
 
    # Sum up the squares
    sum1Sq=sum(pow(prefs[p1][item],2) for item in si)
    sum2Sq=sum(pow(prefs[p2][item],2) for item in si)
 
    # Sum up the products
    pSum=sum(prefs[p1][item]*prefs[p2][item] for item in si)
 
    # Caculate Pearson Score
    num=pSum-(sum1*sum2/n)
    den=sqrt((sum1Sq-pow(sum1,2)/n)*(sum2Sq-pow(sum2,2)/n))
    if den == 0: return 0
    r=num/den
    return r
# print (sim_pearson(critics,'Lisa Rose','Gene Seymour'))


# *****This function will find the best matches for person specified
def topMatches(prefs,person,n=5,similarity=sim_pearson):
 scores=[(similarity(prefs,person,other),other) for other in prefs if other!=person]
 scores.sort()
 scores.reverse()
 return scores[0:n]
# print (topMatches(critics,'Lisa Rose',3,sim_distance))
# print (topMatches(critics,'Lisa Rose',3,sim_pearson))

# *****Recommending item
def getRecommendations(prefs,person,similarity=sim_pearson):
    #Get recommendation for a person by using a weighted average of every other user's ranking
    total={}
    simSums={}
    for other in prefs:
        #don't compare to myself
        if other == person: continue
        
        #find the simlarity pearson coefficient
        sim = similarity(prefs,person,other)

        #ingnore scores of zero or lower
        if sim <= 0: continue
        
        #find all movie other watched
        for movie in prefs[other]:
            #dont recommend the movie was viewed
            if movie in prefs[person]: continue
            #Caculate the rating * similarity
            total.setdefault(movie,0)
            total[movie]+=sim*prefs[other][movie]
            #Caculator the total simlarity
            simSums.setdefault(movie,0)
            simSums[movie] += sim
    rankings=[(total/simSums[movie],movie) for movie,total in total.items()]
    # Return the sorted list
    rankings.sort()
    rankings.reverse()
    return rankings
# print getRecommendations(critics,'Toby',sim_distance)
# print critics

# *****Matching Product
def transformPrefs(prefs):
    result = {}
    for person in prefs:
        for item in prefs[person]:
            result.setdefault(item,{})
            #Flip item and person
            result[item][person] = prefs[person][item]
    return result

# movie = transformPrefs(critics)
# print movie
# print movie['Lady in the Water']
# print topMatches(movie,'Superman Returns')
# print getRecommendations(movie,'Just My Luck')

# *****Item-Based Filtering - Building the item comparison Dataset
def caculateSimilarItem(prefs,n=10):
    # Create a dictionary of items showing which other item they are most similar to
    result={} #dictionary

    # Invert the preference matrix to be item-centric
    itemPrefs = transformPrefs(prefs)
    # this variable use to update status of the build dataset operation
    c = 0
    for item in itemPrefs:
        c += 1
        if c%100 == 0: print "%d/%d" % (c,len(itemPrefs))
        # Find the most similar item to this one
        scores = topMatches(itemPrefs,item,n=n,similarity=sim_distance)
        result[item] = scores
    return result

# top = topMatches(critics,'Toby')
# print top
# print top[0]
# print caculateSimilarItem(critics)
# print critics['Lisa Rose'].items()


# *****Item-based Filtering - Getting Recommendations
def getRecommendedItems(prefs,itemMatch,user):
    userRatings = prefs[user]
    scores = {}
    totalSim = {}
    # Loop over items rated by this user
    for useritem in userRatings:
        for (similarity,item) in itemMatch[useritem]:
            if item in userRatings: continue
            scores.setdefault(item,0)
            scores[item] += similarity * userRatings[useritem]
            totalSim.setdefault(item,0)
            totalSim[item] += similarity
    # Devide each total score by total weighting to get average
    rankings = [(scores/totalSim[item],item) for item,scores in scores.items()]
    # Return the rankings from the highest to lowest
    rankings.sort()
    rankings.reverse()
    return rankings
# print getRecommendedItems(critics,caculateSimilarItem(critics),'Toby')

# *****Using the MovieLens Dataset
def loadMovieLens(path = 'F:/PythonProject/Collective_Intelligence/Data/MovieLens/ml-100k'):
    # Get the movie titles
    movies = {}
    for line in open(path+'/u.item'):
        (id,title) = line.split('|')[0:2]
        movies[id] = title
    # Load data
    prefs = {}
    for line in open(path+'/u.data'):
        (user,movieid,rating,ts) = line.split('\t')
        prefs.setdefault(user,{})
        prefs[user][movies[movieid]] = float(rating)
    return prefs
prefs = loadMovieLens()
# print prefs['87']
# recomend = getRecommendations(prefs,'87')[0:30]
# print recomend
# itemsim = caculateSimilarItem(prefs,n=50)
# print getRecommendedItems(prefs,itemsim,'87')[0:30]
