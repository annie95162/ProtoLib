import numpy as np
import protolib


def print_topk(title, ids, vals, labels):
    print(f"\n{title}")
    for rank, (idx, val) in enumerate(zip(ids, vals), start=1):
        print(f"  Top {rank}: index={idx}, label={labels[idx]}, value={val:.6f}")


def predict_label(top_ids, labels):
    return labels[top_ids[0]]


def main():
    # ------------------------------------------------------------
    # Stage 1: existing classes in memory
    # ------------------------------------------------------------
    print("=== ProtoLib Incremental Learning Demo ===")

    dim = 4
    store = protolib.VectorStore(dim)

    # Prototype memory for old classes
    stage1_vectors = np.array([
        [1.0, 0.0, 0.0, 0.0],   # class A prototype
        [0.0, 1.0, 0.0, 0.0],   # class B prototype
        [0.0, 0.0, 1.0, 0.0],   # class C prototype
    ], dtype=np.float32)

    labels = ["A", "B", "C"]

    store.add(stage1_vectors)

    print("\nStage 1 memory:")
    print(store.data_view())

    # Query close to class A
    query1 = np.array([0.9, 0.1, 0.0, 0.0], dtype=np.float32)

    ids_l2, vals_l2 = store.search_l2(query1, 3)
    ids_cos, vals_cos = store.search_cosine(query1, 3)

    print_topk("Stage 1 - L2 search", ids_l2, vals_l2, labels)
    print_topk("Stage 1 - Cosine search", ids_cos, vals_cos, labels)

    pred_l2_stage1 = predict_label(ids_l2, labels)
    pred_cos_stage1 = predict_label(ids_cos, labels)

    print(f"\nPredicted label with L2 before increment: {pred_l2_stage1}")
    print(f"Predicted label with Cosine before increment: {pred_cos_stage1}")

    # ------------------------------------------------------------
    # Stage 2: add new class incrementally
    # ------------------------------------------------------------
    print("\n=== Adding a new class incrementally ===")

    new_class_vector = np.array([
        [0.9, 0.1, 0.0, 0.0],   # class D prototype
    ], dtype=np.float32)

    store.add(new_class_vector)
    labels.append("D")

    print("\nStage 2 memory after incremental insertion:")
    print(store.data_view())

    ids_l2_after, vals_l2_after = store.search_l2(query1, 4)
    ids_cos_after, vals_cos_after = store.search_cosine(query1, 4)

    print_topk("Stage 2 - L2 search", ids_l2_after, vals_l2_after, labels)
    print_topk("Stage 2 - Cosine search", ids_cos_after, vals_cos_after, labels)

    pred_l2_stage2 = predict_label(ids_l2_after, labels)
    pred_cos_stage2 = predict_label(ids_cos_after, labels)

    print(f"\nPredicted label with L2 after increment: {pred_l2_stage2}")
    print(f"Predicted label with Cosine after increment: {pred_cos_stage2}")

    # ------------------------------------------------------------
    # Additional example: another query
    # ------------------------------------------------------------
    query2 = np.array([0.0, 0.2, 0.8, 0.0], dtype=np.float32)

    ids_l2_q2, vals_l2_q2 = store.search_l2(query2, 3)
    ids_cos_q2, vals_cos_q2 = store.search_cosine(query2, 3)

    print_topk("Query 2 - L2 search", ids_l2_q2, vals_l2_q2, labels)
    print_topk("Query 2 - Cosine search", ids_cos_q2, vals_cos_q2, labels)

    print("\nDemo summary:")
    print("- ProtoLib stores class prototypes incrementally.")
    print("- Search can be done with either L2 distance or cosine similarity.")
    print("- After inserting a new prototype, the retrieval results immediately reflect the updated memory.")
    print("- This demonstrates how ProtoLib can serve as a lightweight prototype memory for incremental learning.")


if __name__ == "__main__":
    main()
