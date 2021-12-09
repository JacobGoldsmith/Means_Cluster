This is a mean based clustering method. 
The algorithm reads vectors from file of vectors seperated by line whose elements are seperated by commas. 
Then runs max-it times or until groups don't change more than epsilon:
  Reassigns all vectors to one of k clusters by closest norm to cluster representative.
  Recalculates the cluster representative by mean.
Returns cluster representatives.

Run this program in Python or C and supply 3 or 4 arguments:
k - Number of clusters you wish to group your vectors by.
max-it - The maximum number of iterations you wish to run. Default is 200
input_file.txt - Text file with vectors you wish to cluster. Each vector ends with new line, each element in vector seperated by comma. 
output_file.txt - The path of the text file to which the program will write the output cluster representatives.
