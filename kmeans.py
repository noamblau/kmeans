import sys

AMOUNT = 0
SUM = 1


def read_vectors_from_file():
    vectors = []
    while True:
        vector = []
        try:
            for num in input().strip('\n').split(','):
                vector.append(float(num))
        except EOFError:
            break
        if len(vector) != 0:
            vectors.append(vector)
    return vectors


def get_vectors_dim(vectors):
    return len(vectors[0])


def get_vectors_num(vectors):
    return len(vectors)


def init_centroids(vectors, k, dim):
    centroids = []
    for i in range(k):
        cent = []
        for j in range(dim):
            cent.append(vectors[i][j])
        centroids.append(cent)
    return centroids


def average_vector(vectors, dim, n):
    centroids = init_centroids(vectors, 1, dim)
    for i in range(1, n):
        for j in range(dim):
            centroids[0][j] += vectors[i][j]
    for k in range(dim):
        centroids[0][k] = centroids[0][k]/n
    return centroids


def distance_sq(v1, v2):
    distance = 0
    for i in range(len(v1)):
        distance += (v1[i]-v2[i])**2
    return distance


def init_clusters(k):
    clusters = []
    for index in range(k):
        clusters.append([0, [0] * dimension])
    return clusters


def reset_clusters(clusters, dim, k):
    for i in range(k):
        clusters[i][AMOUNT] = 0
        for j in range(dim):
            clusters[i][SUM][j] = 0


def calculate_clusters(vectors, centroids, clusters, dim, k):
    for vector in vectors:
        min_distance = distance_sq(vector, centroids[0])
        closest_cluster = 0
        for i in range(1, k):
            distance = distance_sq(vector, centroids[i])
            if distance < min_distance:
                min_distance = distance
                closest_cluster = i
        clusters[closest_cluster][AMOUNT] += 1
        for j in range(dim):
            clusters[closest_cluster][SUM][j] += vector[j]


# return how many centroids have changed

def update_centroids(clusters, centroids, dim, k):
    changed = False
    for i in range(k):
        for j in range(dim):
            new_cor = clusters[i][SUM][j]/clusters[i][AMOUNT]
            if centroids[i][j] != new_cor:
                changed = True
                centroids[i][j] = new_cor
    return changed


def print_vector(v):
    for i in range(len(v) - 1):
        print("{0:.4f}".format(v[i]), end=",")
    print("{0:.4f}".format(v[len(v)-1]))


if __name__ == "__main__":
    if len(sys.argv) > 3 or len(sys.argv) < 2:
        raise Exception("Invalid number of arguments")
    vectors_list = read_vectors_from_file()
    try:
        k_val = int(sys.argv[1])
    except ValueError:
        raise Exception("Invalid k")
    dimension = get_vectors_dim(vectors_list)
    max_iter = 200
    if len(sys.argv) == 3:
        try:
            max_iter = int(sys.argv[2])
        except ValueError:
            raise Exception("Invalid number of iterations")
    v_num = get_vectors_num(vectors_list)
    if k_val <= 0 or k_val >= v_num:
        raise Exception("Invalid k")
    if max_iter <= 0:
        raise Exception("Invalid number of iterations")
    if k_val != 1:
        clusters_list = init_clusters(k_val)
        centroids_list = init_centroids(vectors_list, k_val, dimension)
        iter_num = 0
        centroids_changed = True
        while (iter_num < max_iter) and centroids_changed:
            calculate_clusters(vectors_list, centroids_list, clusters_list, dimension, k_val)
            centroids_changed = update_centroids(clusters_list, centroids_list, dimension, k_val)
            reset_clusters(clusters_list, dimension, k_val)
            iter_num += 1
    else:
        centroids_list = average_vector(vectors_list, dimension, v_num)
    for centroid in centroids_list:
        print_vector(centroid)
