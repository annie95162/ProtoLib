import numpy as np

def l2_search(data, query, k):
    dists = np.linalg.norm(data - query, axis=1)
    order = np.lexsort((np.arange(len(dists)), dists))
    order = order[:min(k, len(order))]
    return order.tolist(), dists[order].tolist()

def cosine_search(data, query, k):
    qnorm = np.linalg.norm(query)
    if qnorm == 0:
        raise ValueError("zero query vector")

    sims = []
    for i, vec in enumerate(data):
        vnorm = np.linalg.norm(vec)
        if vnorm == 0:
            sims.append(0.0)
        else:
            sims.append(float(np.dot(query, vec) / (qnorm * vnorm)))

    sims = np.array(sims, dtype=np.float32)
    order = np.lexsort((np.arange(len(sims)), -sims))
    order = order[:min(k, len(order))]
    return order.tolist(), sims[order].tolist()

if __name__ == "__main__":
    data = np.array([
        [1.0, 0.0],
        [0.0, 1.0],
        [2.0, 0.0],
        [1.0, 1.0],
    ], dtype=np.float32)

    query = np.array([1.0, 0.0], dtype=np.float32)

    print("L2:", l2_search(data, query, 4))
    print("Cosine:", cosine_search(data, query, 4))
