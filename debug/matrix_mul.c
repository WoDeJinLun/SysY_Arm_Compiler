const int MAX_SIZE = 25;

int A[MAX_SIZE][MAX_SIZE];
int B[MAX_SIZE][MAX_SIZE];
int C[MAX_SIZE][MAX_SIZE];

void matrix_multiply(int M_A, int N_B, int N_A) {
    int i = 0;
    while (i < M_A) {
        int j = 0;
        while (j < N_B) {
            C[i][j] = 0;
            int k = 0;
            while (k < N_A) {
                C[i][j] = C[i][j] + A[i][k] * B[k][j];
                k = k+1;
            }
            j = j+1;
        }
        i = i+1;
    }
    return ;
}

int main() {
    // 读取矩阵A的维度
    // scanf("%d %d", &M_A, &N_A);
    int M_A;
    int N_A;
    int M_B;
    int N_B;

    M_A = getint();
    N_A = getint();

    // 读取矩阵A的元素
    int i = 0;
    while (i < M_A) {
        int j = 0;
        while (j < N_A) {
            A[i][j] = getint();
            // scanf("%d", &A[i][j]);
            j = j+1;
        }
        i = i+1;
    }

    // 读取矩阵B的维度
    // scanf("%d %d", &M_B, &N_B);
    M_B = getint();
    N_B = getint();

    // 读取矩阵B的元素
    i = 0;
    while (i < M_B) {
        int k = 0;
        while (k < N_B) {
            // scanf("%d", &B[i][j]);
            B[i][k] = getint();
            k = k+1;
        }
        i = i+1;
    }

    // 判断维度是否满足乘法要求
    if (N_A != M_B) {
        // putf("Incompatible Dimensions\n");
        putch(73);
        putch(110);
        putch(99);
        putch(111);
        putch(109);
        putch(112);
        putch(97);
        putch(116);
        putch(105);
        putch(98);
        putch(108);
        putch(101);
        putch(32);
        putch(68);
        putch(105);
        putch(109);
        putch(101);
        putch(110);
        putch(115);
        putch(10);
        
        return 0;
    }
    // 调用矩阵乘法函数
    matrix_multiply(M_A, N_B, N_A);

    // 输出结果
    i = 0;
    while (i < M_A) {
        int j = 0;
        while (j < N_B) {
            putint(C[i][j]);
            j = j+1;
        }
        putch(10);
        i = i+1;
    }

    return 0;
}