import random

class SegmentTree:
    def __init__(self, arr):
        self.n = len(arr)
        self.tree = [float('inf')] * (4 * self.n)  # Initialize with infinity
        self.build(arr, 1, 0, self.n - 1)

    def build(self, arr, node, left, right):
        if left == right:
            self.tree[node] = arr[left]
        else:
            mid = (left + right) // 2
            self.build(arr, 2 * node, left, mid)
            self.build(arr, 2 * node + 1, mid + 1, right)
            self.tree[node] = min(self.tree[2 * node], self.tree[2 * node + 1])

    def query(self, node, left, right, query_left, query_right):
        if query_left > right or query_right < left:
            return float('inf')
        if query_left <= left and query_right >= right:
            return self.tree[node]
        mid = (left + right) // 2
        return min(self.query(2 * node, left, mid, query_left, query_right),
                   self.query(2 * node + 1, mid + 1, right, query_left, query_right))

    def update(self, node, left, right, index, value):
        if index < left or index > right:
            return
        if left == right:
            self.tree[node] = value
        else:
            mid = (left + right) // 2
            self.update(2 * node, left, mid, index, value)
            self.update(2 * node + 1, mid + 1, right, index, value)
            self.tree[node] = min(self.tree[2 * node], self.tree[2 * node + 1])


class QueueManager:
    def __init__(self, num_queues=8, queue_size=8):
        self.num_queues = num_queues
        self.queue_size = queue_size
        self.queues = [[] for _ in range(num_queues)]
        self.fullness = [0] * self.num_queues
        self.bitmap = 0
        self.alpha = alpha

        initial_scores = [0] * self.num_queues  
        self.segment_tree = SegmentTree(initial_scores)

    def enqueue(self, queue_id, item):
        if len(self.queues[queue_id]) < self.queue_size:
            self.queues[queue_id].append(item)
            self.fullness[queue_id] += 1
            self.update_score(queue_id)

            if self.fullness[queue_id] == self.queue_size:
                self.bitmap |= (1 << queue_id)  # Set the bit to 1 (full)
        else:
            print(f"Queue {queue_id} is full, adding to next level")

    def find_overflow_queue(self, full_queue_id):
        min_score = float('inf')
        overflow_queue_id = -1

        for i in range(full_queue_id):
            # Check if the queue is not full using the bitmap
            if (self.bitmap >> i) & 1 == 0 and (self.num_queues - i) * self.fullness[i] < min_score:
                min_score = (self.num_queues - i) * self.fullness[i]
                overflow_queue_id = i

        return overflow_queue_id

    def update_score(self, queue_id):
        distance = self.num_queues - queue_id
        fullness = self.fullness[queue_id]
        new_score = (1 - self.alpha) * distance + self.alpha * fullness 
        self.segment_tree.update(1, 0, self.num_queues - 1, queue_id, new_score)

    def query_min(self, right): 
        return self.segment_tree.query(1, 0, self.num_queues - 1, 0, right)

# Example Usage
num_queues = 8
queue_size = 4 
alpha = 0.3 

qm = QueueManager(num_queues, queue_size)

for _ in range(30):
    rank = random.randint(0, num_queues - 1)
    print(f"Generated rank: {rank}")

    if qm.fullness[rank] == queue_size:
        overflow_queue_id = qm.find_overflow_queue(rank)
        if overflow_queue_id != -1:
            print(f"Queue {rank} is full, adding to overflow queue {overflow_queue_id}")
            qm.enqueue(overflow_queue_id, rank) 
        else:
            print(f"Queue {rank} is full, adding to next level")
    else:
        print(f"Adding to queue {rank}")
        qm.enqueue(rank, rank)

    print("Queue Fullness:", qm.fullness)
    print("-" * 20)