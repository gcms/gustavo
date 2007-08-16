
/*-----------------------prototipos.h-------------------------

 Este arquivo contem as declaracoes dos prototipos das funcoes
 utilizadas que sao chamadas no corpo do programa principal

-------------------------------------------------------------*/ 

extern void ledado(int,int []);
extern void fcarga(double[],double[]);
extern void prepara(void);
extern void orden(void);
extern void monta_mat(double[],double[]);
extern void redu(double[]);
//extern void solu(double[],double[]);
extern void estado(double[],double[]);
extern void fluxos(void);
extern void fluxpq(void);
extern void sobrecarga(void);
extern void relat1(void);
extern void relat2(void);
extern void relat3(void);
extern void relat4(void);
extern void relat5(void);
extern void relat6(void);
extern void relat7(void);
extern void escolha(int);
extern void salida(int);
extern void output(void);
extern void scrdireto_reativo(double[],double[],int, int);
extern void identdc(int,int);
extern void identca(int,int,int);
extern void grafo(void);
extern void caminho(int,int);
extern int  bubble(int[],int);
extern void soluve(double[],double[]);
extern void soluve2(double[],double[]);

extern void soluffs(double[],double[]);
extern void solufs(double[],double[]);
extern void solubs(double[],double[]);
extern void bennett(double[],double[],double[]);
extern void prodMdelta_teta(double[],double[],int,int);
extern void ricupero(int);
extern void modify(int,int,int);
extern void multmv1(double[][],double[],int,double[]);
extern void multmv2(double[][],double[],int,int,double[]);
extern void multmv3(double[],int[],int[],double[],int,double[]);
extern void multmv4(double[][],double[],int,int,double[]);
extern void multmm1(double[][],double[][],int,double[][]);
extern void multmm2(double[][],double[][],int,int,double[][]);
extern void multmm3(double[][],double[][],int,int,double[][]);
extern void multmm4(double[],int[],int[],double[][],int,double[][]);
extern void inversa(double[][],int);
extern void cont_pqdir(double[],double[],int,int,int);
extern void sobretensao(void);
extern void rank2(int []);
extern void reordena(void);
extern void contingpq_multiple(double[],double[],int,int,int);
extern void refatorapq(double[],double[],int,int,int);
extern void ordena(int,double[],int[],int);

extern void ordena2(int,double[],int[],int);
extern void potencia(void);
extern void solu(double[],double[]);

extern void contingenciaAC(double[],double[],int,int,int);
extern void conting_single(double[],double[],int,int,int,int);
extern void conting_multiple(double[],double[],int,int,int,int);
extern void refatora(double[],double[],int,int,int);
extern void state_pos(double[],double[],int,int,int,double[],double[], int);
extern void state_mid(double[],double[],int,double[],double[],int);
extern void state_normal(double[],double[],int,int);
// Funcoes relacionadas com Programacao Paralela
extern void inicio();
extern void MestreA(double[],double[],int);
extern void EscravoA(double[],double[],int);
extern void EscravoScreening(double[],double[],int);
extern void MestreScreening(double[],double[],int);
