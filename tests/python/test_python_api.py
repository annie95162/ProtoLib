import numpy as np
import pytest
import protolib

def test_create_store():
    store = protolib.VectorStore(3)
    assert store.dim() == 3
    assert store.size() == 0

def test_add_vectors():
    store = protolib.VectorStore(3)
    data = np.array([[1, 2, 3], [4, 5, 6]], dtype=np.float32)

    store.add(data)
    assert store.size() == 2

def test_add_wrong_dimension():
    store = protolib.VectorStore(3)
    data = np.array([[1, 2], [3, 4]], dtype=np.float32)

    with pytest.raises(Exception):
        store.add(data)

def test_add_wrong_dtype():
    store = protolib.VectorStore(3)
    data = np.array([[1, 2, 3]], dtype=np.float64)

    with pytest.raises(Exception):
        store.add(data)

def test_add_noncontiguous():
    store = protolib.VectorStore(2)
    base = np.array([[1, 2], [3, 4], [5, 6]], dtype=np.float32)
    data = base[:, ::-1]

    with pytest.raises(Exception):
        store.add(data)

def test_search_l2():
    store = protolib.VectorStore(2)
    data = np.array([[1, 1], [4, 5], [2, 2]], dtype=np.float32)
    store.add(data)

    query = np.array([1, 1], dtype=np.float32)
    ids, vals = store.search_l2(query, 2)

    assert ids[0] == 0
    assert np.isclose(vals[0], 0.0)

def test_search_cosine():
    store = protolib.VectorStore(2)
    data = np.array([[1, 0], [0, 1], [2, 0]], dtype=np.float32)
    store.add(data)

    query = np.array([1, 0], dtype=np.float32)
    ids, vals = store.search_cosine(query, 2)

    assert ids[0] == 0
    assert np.isclose(vals[0], 1.0)
    assert ids[1] == 2
    assert np.isclose(vals[1], 1.0)

def test_search_wrong_dtype():
    store = protolib.VectorStore(2)
    data = np.array([[1, 0], [0, 1]], dtype=np.float32)
    store.add(data)

    query = np.array([1, 0], dtype=np.float64)
    with pytest.raises(Exception):
        store.search_l2(query, 1)

def test_search_noncontiguous():
    store = protolib.VectorStore(2)
    data = np.array([[1, 0], [0, 1]], dtype=np.float32)
    store.add(data)

    base = np.array([[1, 0], [2, 0]], dtype=np.float32)
    query2d = base[:, 0]
    assert not query2d.flags["C_CONTIGUOUS"] or query2d.ndim == 1

    if not query2d.flags["C_CONTIGUOUS"]:
        with pytest.raises(Exception):
            store.search_l2(query2d, 1)

def test_data_view_shape_and_content():
    store = protolib.VectorStore(3)
    data = np.array([[1, 2, 3], [4, 5, 6]], dtype=np.float32)
    store.add(data)

    view = store.data_view()
    assert view.shape == (2, 3)
    assert view.dtype == np.float32
    assert np.allclose(view, data)

def test_data_view_is_zero_copy_read_view():
    store = protolib.VectorStore(2)
    data = np.array([[1, 2], [3, 4]], dtype=np.float32)
    store.add(data)

    view1 = store.data_view()
    view2 = store.data_view()

    assert np.shares_memory(view1, view2)
    assert np.allclose(view1, data)
