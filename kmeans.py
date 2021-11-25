EPSILON = 0.001

def k_means_cluster(k, input_filename, output_filename, max_it=200):
    vectors = collect(input_filename)
    vec_size = len(vectors[0])

    centroids = vectors[:k]

    for i in range(max_it):
        # initialize lists to calculate new centroids later
        sums = [[0 for j in range(vec_size)] for h in range(k)]
        counts = [0 for h in range(k)]

        for v in vectors:
            cent_ind = find_cent_ind(v, centroids)

            # update sums and counts
            sums[cent_ind] = add_vecs(sums[cent_ind], v)
            counts[cent_ind] += 1

        smaller_than_e = update_centroids(k, vec_size, centroids, sums, counts)

        if smaller_than_e:
            break

    write_out_file(output_filename, centroids)

    return


# returns list of vectors as floats
def collect(input_filename):
    file = open(input_filename)
    lines = file.readlines()
    vectors = []
    for line in lines:
        vec_strings = line.split(',')
        vec_floats = [float(s) for s in vec_strings]
        vectors.append(vec_floats)
    file.close()
    return vectors


# returns index of closest centroid to v
def find_cent_ind(v, centroids):
    minimum_ind = 0
    minimum_delta = delta_norm_pow2(v, centroids[0], len(v))
    for ind, c in enumerate(centroids):
        delta = delta_norm_pow2(v, c, len(v))
        if delta < minimum_delta:
            minimum_ind = ind
            minimum_delta = delta
    return minimum_ind


def add_vecs(v1, v2):
    return [x1 + x2 for x1, x2 in zip(v1, v2)]


def update_centroids(k, vec_size, centroids, sums, counts):
    smaller_than_e = True

    for i in range(k):  # update all k centroids
        prev_cent = centroids[i]  # temporarily save prev centroid
        centroids[i] = [s / counts[i] for s in sums[i]]  # update centroid

        # update smaller_than_e
        delta_norm = delta_norm_pow2(prev_cent, centroids[i], vec_size) ** 0.5

        if (delta_norm >= EPSILON):
            smaller_than_e = False



    return smaller_than_e


def delta_norm_pow2(v1, v2, vec_size):
    res = 0

    for i in range(vec_size):
        delta = v1[i] - v2[i]
        res += delta ** 2

    return res


def write_out_file(output_filename, centroids):
    f = open(output_filename, 'w')

    for cent in centroids:
        for ind, c in enumerate(cent):
            f.write(str("%.4f" % c))
            if ind < len(cent) - 1:
                f.write(',')
        f.write('\n')
    f.close()
    return


#########test#########

k_means_cluster(3, "input_1.txt", "test_output_1.txt", max_it=600)

k_means_cluster(7, "input_2.txt", "test_output_2.txt")

k_means_cluster(15, "input_3.txt", "test_output_3.txt", max_it=300)
