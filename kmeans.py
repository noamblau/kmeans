import sys

AMOUNT = 0
SUM = 1


def read_file():
    vectors = []
    while True:
        vector = []
        try:
            # i= 0
            for num in input().strip('\n').split(','):
                # i+=1;
                vector.append(float(num))
                # if i<4:
                #     print(num)
        except EOFError:
            break
        if len(vector) != 0:
            vectors.append(vector)
    # print(vectors)
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
        min_distance = float('inf')
        closest_cluster = 0
        for i in range(k):
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
    # print(centroids)
    # print(changed)
    return changed


def print_vector(v):
    for i in range(len(v) - 1):
        print("{0:.4f}".format(v[i]), end=",")
    print("{0:.4f}".format(v[len(v)-1]))


def print_vector_delete(v):
    for i in range(len(v)-1):
        [n, q] = str(v[i]).split(".")
        print(n + "." + q[:4], end=",")
    [n, q] = str(v[len(v)-1]).split(".")
    print(n + "." + q[:4])


if __name__ == "__main__":
    vectors_list = read_file()
    k_val = int(sys.argv[1])
    dimension = get_vectors_dim(vectors_list)
    max_iter = 200
    if len(sys.argv) == 3:
        max_iter = int(sys.argv[2])
    clusters_list = init_clusters(k_val)
    # print(clusters_list)
    centroids_list = init_centroids(vectors_list, k_val, dimension)
    # print(centroids_list)
    iter_num = 0
    centroids_changed = True
    while (iter_num < max_iter) and centroids_changed:
        calculate_clusters(vectors_list, centroids_list, clusters_list, dimension, k_val)
        centroids_changed = update_centroids(clusters_list, centroids_list, dimension, k_val)
        # print(clusters_list)
        # print(centroids_list)
        reset_clusters(clusters_list, dimension, k_val)
        iter_num += 1
    # print(iter_num)
    # print(len(centroids_list))
    for centroid in centroids_list:
        print_vector(centroid)
