import matplotlib.pyplot as plt

def hofstadter_q(n):
    Q = [None, 1, 1]
    for i in range(3, n+1):
        Q.append(Q[i - Q[i-1]] + Q[i - Q[i-2]])
    return Q[1:]

n = 100
q_seq = hofstadter_q(n)
plt.figure(figsize=(10, 5))
plt.plot(range(1, n+1), q_seq, marker='o', linestyle='-')
plt.title("Hofstadter Q-sequence (first 100 terms)")
plt.xlabel("n")
plt.ylabel("Q(n)")
plt.grid(True)
plt.show()
