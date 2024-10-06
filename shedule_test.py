import random
import time

class SegmentTree:
    def __init__(self, arr):
        self.n = len(arr)
        self.tree = [float('inf')] * (4 * self.n)
        self.tree_index = [0] * (4 * self.n)
        self.build(arr, 1, 0, self.n - 1)

    def build(self, arr, node, left, right):
        if left == right:
            self.tree[node] = arr[left]
            self.tree_index[node] = left
        else:
            mid = (left + right) // 2
            self.build(arr, 2 * node, left, mid)
            self.build(arr, 2 * node + 1, mid + 1, right)
            if self.tree[2 * node] < self.tree[2 * node + 1]:
                self.tree[node] = self.tree[2 * node]
                self.tree_index[node] = self.tree_index[2 * node]
            else:
                self.tree[node] = self.tree[2 * node + 1]
                self.tree_index[node] = self.tree_index[2 * node + 1]

    def query(self, node, left, right, query_left, query_right):
        if query_left > right or query_right < left:
            return float('inf'), -1
        if query_left <= left and query_right >= right:
            return self.tree[node], self.tree_index[node]
        mid = (left + right) // 2
        left_min, left_index = self.query(2 * node, left, mid, query_left, query_right)
        right_min, right_index = self.query(2 * node + 1, mid + 1, right, query_left, query_right)
        if left_min < right_min:
            return left_min, left_index
        else:
            return right_min, right_index

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
    def __init__(self, num_queues=8, queue_size=8, alpha=0.3):
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
                self.bitmap |= (1 << queue_id)
        else:
            print(f"Queue {queue_id} is full, adding to next level")

    def dequeue(self):
        current_dequeue_queue = 0

        while True:
            if self.queues[current_dequeue_queue]:
                item = self.queues[current_dequeue_queue].pop(0)
                self.fullness[current_dequeue_queue] -= 1
                self.update_score(current_dequeue_queue)

                if self.fullness[current_dequeue_queue] == 0:
                    self.bitmap &= ~(1 << current_dequeue_queue)

                print(f"Dequeued item: {item} from queue {current_dequeue_queue}")
                return item

            print(f"Queue {current_dequeue_queue} is empty.")
            current_dequeue_queue = (current_dequeue_queue + 1) % self.num_queues

            if current_dequeue_queue == 0:
                print("All queues are empty.")
                return None

    def shift_queue_to_highest_rank(self):
        temp_queue = self.queues.pop(0)
        self.queues.append(temp_queue)

        temp_fullness = self.fullness.pop(0)
        self.fullness.append(temp_fullness)

        if temp_fullness == self.queue_size:
            self.bitmap |= (1 << (self.num_queues - 1))
        else:
            self.bitmap &= ~(1 << (self.num_queues - 1))

        for i in range(self.num_queues):
            self.update_score(i)

    def find_overflow_queue(self, full_queue_id):
        _, overflow_queue_id = self.segment_tree.query(1, 0, self.num_queues - 1, 0, full_queue_id - 1)
        if overflow_queue_id != -1 and (self.bitmap >> overflow_queue_id) & 1 == 0:
            return overflow_queue_id
        return -1

    def update_score(self, queue_id):
        distance = self.num_queues - queue_id
        fullness = self.fullness[queue_id]
        new_score = (1 - self.alpha) * distance + self.alpha * fullness
        self.segment_tree.update(1, 0, self.num_queues - 1, queue_id, new_score)

    def print_scores(self):
        scores = []
        for i in range(self.num_queues):
            distance = self.num_queues - i
            fullness = self.fullness[i]
            score = (1 - self.alpha) * distance + self.alpha * fullness
            scores.append(score)
        print("Queue Scores:", scores)

# Example Usage (Modified for Enqueue/Dequeue Simulation)
num_queues = 4
queue_size = 2
alpha = 0.3
qm = QueueManager(num_queues, queue_size, alpha)

enqueue_probability = 0.6  
dequeue_probability = 0.3

for iteration in range(1, 31): 
    print(f"\n--- Iteration {iteration} ---")

    if random.random() < enqueue_probability:
        rank = random.randint(0, num_queues - 1)
        print(f"Generated rank: {rank}")

        if qm.fullness[rank] == queue_size:
            for i in range(qm.num_queues):
                qm.update_score(i)
            overflow_queue_id = qm.find_overflow_queue(rank)
            if overflow_queue_id != -1:
                print(f"Queue {rank} is full, adding to overflow queue {overflow_queue_id}")
                qm.enqueue(overflow_queue_id, rank)
            else:
                print(f"Queue {rank} is full, adding to next level")
        else:
            print(f"Adding to queue {rank}")
            qm.enqueue(rank, rank)

    if random.random() < dequeue_probability:
        dequeued_item = qm.dequeue()
        if dequeued_item is not None:
            if qm.fullness[0] == 0 and qm.queues[0] == []:
                print("Shifting queue 0 to highest rank...")
                qm.shift_queue_to_highest_rank()

    qm.print_scores()
    print("Queue Fullness:", qm.fullness)
    print("-" * 20)

    time.sleep(0.5) 