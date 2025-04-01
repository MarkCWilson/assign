# processing the simulation output used in
# Order Symmetry: A New Fairness Criterion for Assignment Mechanisms, by Rupert Freeman, Geoffrey Pritchard and Mark C. Wilson
# Found at https://osf.io/preprints/socarxiv/xt37c_v1
# uses fact that we have 1000 iterations at each parameter setting

library(tidyverse)
library(reshape2)

numreps = 5000

get_matrix = function(file) {
    df <- read.csv(file,header=FALSE,skip=0, skipNul=TRUE)
    return(df)
}

get_distmat = function(n) {
    base_direc = "/Users/mwilson/Dropbox/Mark_research/simulation/outputs/numerical_results/FPW2021/"
    #dataset_direc = "matrixintermediate/"
    dataset_direc = "matrixextreme/mallows/rankdist/"
    #dataset_direc = "matrixsquare/"
    direc = paste0(base_direc,dataset_direc,n,"n") 
    print(direc)
    setwd(direc)
    
    f = list.files()
    df = data.frame()
    out = data.frame()
    if (str_detect(dataset_direc, "extreme")) {
        r = n
    }
    else if (str_detect(dataset_direc, "intermediate")) {
        r = log(n,2)     
    }
    else if (str_detect(dataset_direc, "square")) {
        r = sqrt(n)
    }
    
    out<-tibble(.rows=0)
    for (name in f) {
        alg = str_split_fixed(name,"\\_", 2)[1,2]
        phi = str_split_fixed(name,"\\_", 2)[1,1]
        df <- get_matrix(name)
        # df <- df[,1:(ncol(df)-1)] # bad input format for intermediate and square, extra comma
        
        nr = rep(alg, n)
        sr = rep(n, n)
        pr =  rep(as.numeric(phi), n)
        temp <- cbind(nr, sr,pr, df)
        out<-rbind(out,temp)
       # print(out)
    }
    posnames = seq(1,n)
    posnames <- paste("p", posnames, sep = "_")
    #out<-as_tibble(data.frame(out[,1],apply(out[,-1],2,as.numeric)))
    colnames(out) = c("algo", "n", "phi", posnames)
    alarm()
    return(out)
}

bias_frob = function(n) {
  base_direc = "/Users/mwilson/Dropbox/Mark_research/simulation/outputs/numerical_results/FPW2021/"
  #dataset_direc = "matrixintermediate/"
  dataset_direc = "matrixextreme/mallows/rankdist/"
  #dataset_direc = "matrixsquare/"
  direc = paste0(base_direc,dataset_direc,n,"n") 
  print(direc)
  setwd(direc)
  
  f = list.files()
  df = data.frame()
  out = data.frame()
  if (str_detect(dataset_direc, "extreme")) {
    r = n
  }
  else if (str_detect(dataset_direc, "intermediate")) {
    r = log(n,2)     
  }
  else if (str_detect(dataset_direc, "square")) {
    r = sqrt(n)
  }
  
  out<-tibble(.rows=0)
  for (name in f) {
    alg = str_split_fixed(name,"\\_", 2)[1,2]
    phi = str_split_fixed(name,"\\_", 2)[1,1]
    df <- get_matrix(name)
    rtot <- rowSums(df)[1]
    # df <- df[,1:(ncol(df)-1)] # bad input format for intermediate and square, extra comma
    D<- as.matrix(df)
    Dbar<- colMeans(df)
    Dbar<-t(replicate(nrow(D), Dbar))
     frob <- norm(D-Dbar, "f")/sqrt(n)/rtot
    temp <- cbind(alg, as.numeric(n),as.numeric(phi), as.numeric(frob))
    out<-rbind(out,temp)
    # print(out)
  }
  
  colnames(out) = c("algo", "n", "phi", "frob")
  alarm()
  return(out)
  
}

make_util = function(n) { # derive utility of each agent from rank matrix 
    base_direc = "/Users/mwilson/Dropbox/Mark_research/simulation/outputs/numerical_results/FPW2021/"
    #dataset_direc = "matrixintermediate/"
    dataset_direc = "matrixextreme/mallows/rankdist/"
    #dataset_direc = "matrixsquare/"
    direc = paste0(base_direc,dataset_direc,n,"n/") 
    print(direc)
    setwd(direc)
    
    f = list.files()
    df = data.frame()
    out = data.frame()
    if (str_detect(dataset_direc, "extreme")) {
        r = n
    }
    else if (str_detect(dataset_direc, "intermediate")) {
        r = log(n,2)     
    }
    else if (str_detect(dataset_direc, "square")) {
        r = sqrt(n)
    }
    
    ub <- matrix(seq(r-1,0,by = -1),ncol=1) # borda
    ub<- ub/(r-1)
    ub<- as.numeric(as.matrix(ub,ncol=1))
    
    up <- c(rep(1,1),rep(0,r-1)) # plurality
    up<- up
    up<- as.numeric(as.matrix(up,ncol=1))
    
    d = min(2,n)
    u2<- c(rep(1,d),rep(0,r-d)) #2-approval
    u2<- u2
    u2<- as.numeric(as.matrix(u2,ncol=1))
    
    d = min(3,n)
    u3<- c(rep(1,d),rep(0,r-d)) #3-approval
    u3<- u3
    u3<- as.numeric(as.matrix(u3,ncol=1))
    #p0
    
    ua <- c(rep(1,r-1), rep(0,1)) #antiplurality
    ua<- ua
    ua<- as.numeric(as.matrix(ua,ncol=1))
    
   # u<-cbind(ub,up,u2) want to do all at once but a bit tricky later
    
    out<-tibble(.rows=0)

    for (name in f) {
        alg = str_split_fixed(name,"\\_", 2)[1,2]
       # print(alg)
        phi = str_split_fixed(name,"\\_", 2)[1,1]
        df <- get_matrix(name)
        
        m <- as.matrix(df)
        w <- m%*%u3 # only doing one utility function at a time here
        w <- as_tibble(w)
        
        row<-as.numeric(cbind.data.frame(n, as.numeric(phi),t(w)))
        row<-cbind.data.frame(c(alg),t(row))
        out <- rbind.data.frame(out,row)
    }
    posnames = seq(1,n)
    posnames <- paste("p", posnames, sep = "_")
    #out<-as_tibble(data.frame(out[,1],apply(out[,-1],2,as.numeric)))
    colnames(out) = c("algo", "n", "phi", posnames)
    alarm()
    return(out)
}

get_util = function(n) {
    base_direc = "/Users/mwilson/Dropbox/Mark_research/simulation/outputs/numerical_results/FPW2021/"
    #dataset_direc = "matrixintermediate/"
    dataset_direc = "matrixextreme/mallows/util/"
    #dataset_direc = "matrixsquare/"
    direc = paste0(base_direc,dataset_direc,n,"n/") 
    print(direc)
    setwd(direc)
    
    f = list.files()
    df = data.frame()
    out = data.frame()
    if (str_detect(dataset_direc, "extreme")) {
        r = n
    }
    else if (str_detect(dataset_direc, "intermediate")) {
        r = log(n,2)     
    }
    else if (str_detect(dataset_direc, "square")) {
        r = sqrt(n)
    }
    
    out<-tibble(.rows=0)
    
    for (name in f) {
        alg = str_split_fixed(name,"\\_", 2)[1,2]
        phi = str_split_fixed(name,"\\_", 2)[1,1]
        df <- get_matrix(name)
        nr = rep(alg, numreps)
        sr = rep(n, numreps)
        pr =  rep(as.numeric(phi), numreps)
        temp <- cbind.data.frame(nr, sr,pr, df)
        out<-rbind.data.frame(out,temp)
    }
    colnames(out) = c("algo", "n", "phi", "ub", "eb", "mb", "nb", "ob", "up", "ep", "mp", "obp")
    alarm()
    return(out)
}

analyse = function(df) {
    print(df)
    df<- df %>% 
        mutate(ma = apply(df[,-c(1:3)],1,FUN=max), mi = apply(df[,-c(1:3)],1,FUN=min), uw = apply(df[,-c(1:3)],1,FUN=mean), ob = ma-mi) 
    #mutate(ma = apply(df[,-c(1:3)],1,FUN=max), mi = apply(df[,-c(1:4)],1,FUN=min), ob = ma - mi, uw = rowMeans(df2), na = rowMeans(df3)) %>% # doesn't make sense since we have alrady averaged over profiles
    #mutate(na = exp(na)) %>% # to get Nash utility right
    #arrange(ob) # sorting 
    return(select(df,"algo","n", "phi", "ma", "mi","ob", "uw"))
}

create = function() {
    t<-data.frame(matrix(ncol=5,nrow=5)) # 5 algorithms
    colnames(t) <- c("algo", "n", "phi", "ob", "uw")
    n = 2
    t<- analyse(make_util(n))
    while(n<32) {
        n<- 2*n
        d<- make_util(n)
        dd<- analyse(d)
        t<- rbind.data.frame(t,dd)
    }
    return(as_tibble(t))
} 

create_util = function() {
    t<-data.frame(matrix(ncol=10,nrow=5)) # 5 algorithms
    colnames(t) <- c("algo", "n", "phi", "ub", "eb", "nb", "ob", "up", "ep", "obp")
    n = 2
    t<- get_util(n)
    while(n<32) {
        n<- 2*n
        d<- get_util(n)
        t<- bind_rows(t,d)
    }
    return(t)
} 

# plots

#algset = c("NB", "AB", "SD")
#algset = c("B", "BRB", "RB", "SD", "TTC")
#algset = c("SD", "TTC")
#algset = c("Bos", "Bob", "Bop")
#algset = c("TTC", "SDN", "SD0", "SD1")
#algset = c("TTC", "Bos", "SDN")
#algset = c("B", "Z", "Bos", "Bob")
algset = c("SD0", "SD1", "SDN", "TTC")
algcols = c("Bos" = "black", "Bcyc" = "black", "Rcyc" = "brown", "Bob" = "brown", "Bop" = "lightgreen", "TTC" = "darkgreen", "SDN" = "red", "B" = "blue", "Bcom" = "blue", "SD1" = "purple", "SD0" = "gold", "RB" = "grey", "Rcom" ="grey")

df_orig<- create()
df <- df_orig %>% filter((20*phi)%%1 == 0)
p0 <- ggplot(data = df %>% filter(algo %in% algset) %>% filter(phi == 1.0), aes(x = n, y = ob/numreps, color = algo, shape = algo)) + scale_color_manual(values = algcols) + theme(text = element_text(size = 18)) + geom_point(size=3) + geom_line() +ggtitle("") + xlab("number of agents") + ylab("order bias") + guides(algo = guide_legend(order=1))
p1 <- ggplot(data = df %>% filter(algo %in% algset) %>% filter(phi == 0.6), aes(x = n, y = ob/numreps, color = algo, shape = algo)) +  scale_color_manual(values = algcols) + theme(text = element_text(size = 18)) + geom_point(size=3) + geom_line() +ggtitle("") + xlab("number of agents") + ylab("order bias") + guides(algo = guide_legend(order=1))
p0w <- ggplot(data = df %>% filter(algo %in% algset) %>% filter(phi == 1.0), aes(x = n, y = uw/numreps, color = algo, shape = algo)) + scale_color_manual(values = algcols) + theme(text = element_text(size = 18)) + geom_point(size=3) + geom_line() +ggtitle("") + xlab("number of agents") + ylab("utilitarian welfare") + guides(algo = guide_legend(order=1))
p1w <- ggplot(data = df %>% filter(algo %in% algset) %>% filter(phi == 0.6), aes(x = n, y = uw/numreps, color = algo, shape = algo)) + scale_color_manual(values = algcols) + theme(text = element_text(size = 18)) + geom_point(size=3) + geom_line() +ggtitle("") + xlab("number of agents") + ylab("utilitarian welfare") + guides(algo = guide_legend(order=1))
s1 <- ggplot(data = df, aes(x = uw/numreps, y = ob/numreps, color = algo, shape = algo)) + geom_point() +  scale_color_manual(values = algcols) + theme(text = element_text(size = 18)) + xlab("utilitarian welfare") + ylab("order bias") # scatter plot of ob vs uw
p2 <- ggplot(data = df %>% filter(algo %in% algset) %>% filter(n == 32), aes(x = phi, y = ob/numreps, color = algo, shape = algo)) + scale_color_manual(values = algcols) + theme(text = element_text(size = 18)) + geom_point(size=3) + geom_line() +ggtitle("") + xlab("Mallows parameter") + ylab("order bias") + guides(algo = guide_legend(order=1))
p2w <- ggplot(data = df %>% filter(algo %in% algset) %>% filter(n == 32), aes(x = phi, y = uw/numreps, color = algo, shape = algo)) + scale_color_manual(values = algcols)+ theme(text = element_text(size = 18)) + geom_point(size=3) + geom_line() +ggtitle("") + xlab("Mallows parameter") + ylab("utilitarian welfare") + guides(algo = guide_legend(order=1))
p0r <- ggplot(data = df %>% filter(algo %in% algset) %>% filter(phi == 1.0), aes(x = n, y = 1+(n-1)*(1-uw/numreps), color = algo, shape = algo))  + scale_color_manual(values = algcols)+ theme(text = element_text(size = 18)) + geom_point(size=3) + geom_line() +ggtitle("") + xlab("number of agents") + ylab("average rank") + guides(algo = guide_legend(order=1))
p1r <- ggplot(data = df %>% filter(algo %in% algset) %>% filter(phi == 0.6), aes(x = n, y = 1+(n-1)*(1-uw/numreps), color = algo, shape = algo)) + scale_color_manual(values = algcols) + theme(text = element_text(size = 18)) + geom_point(size=3) + geom_line() +ggtitle("") + xlab("number of agents") + ylab("average rank") + guides(algo = guide_legend(order=1))

d1<- df %>%  filter(algo %in% algset) %>% group_by(algo) %>% summarize(obm = mean(ob), uwm = mean(uw))
p1d<- ggplot(data = d1, aes(x = algo, y = obm/numreps)) + geom_col(width = 0.5) +ggtitle("")  + scale_color_manual(values = algcols)+ theme(text = element_text(size = 18)) + xlab("algorithm code") + ylab("order bias")
p1dw<- ggplot(data = d1, aes(x = algo, y = uwm/numreps)) + geom_col(width = 0.5) +ggtitle("")  + scale_color_manual(values = algcols)+ theme(text = element_text(size = 18)) + xlab("algorithm code") + ylab("utilitarian welfare")

m<- get_distmat(16)
mm <- m %>%  filter(algo %in% algset) %>%
    filter(row_number() %% 16 == 0) # last agent
mm <- mm[,-c(2:2)]
colnames(mm) = c("algo","phi",seq(1,16))
mmm <- melt(mm, id.vars = c("algo", "phi")) 


pm0<- ggplot(data = mmm %>% filter(algo %in% algset) %>% filter(phi == 1.0), aes(x=as.numeric(variable), y=value/numreps, color = algo, shape = algo)) + scale_color_manual(values = algcols)+ theme(text = element_text(size = 18)) + geom_point(size=3) +geom_line() + xlab("rank received") + ylab("probability")
pm1<- ggplot(data = mmm %>% filter(algo %in% algset) %>% filter(phi == 0.6), aes(x=as.numeric(variable), y=value/numreps, color = algo, shape = algo)) + scale_color_manual(values = algcols)+ theme(text = element_text(size = 18)) + geom_point(size=3) +geom_line() + xlab("rank received") + ylab("probability")

df2 <- make_util(16) %>% select(-n) # need to remove agent number 0 from graph
df2m <- melt(df2, id.vars = c("algo", "phi"))
pf2w <- ggplot(data = df2m %>% filter(algo %in% algset) %>% filter(phi == 1.0), aes(x=as.numeric(variable), y=value/numreps, color = algo, shape = algo)) + scale_color_manual(values = algcols)+ theme(text = element_text(size = 18)) + geom_point(size=3) +geom_line() + xlab("agent number") + ylab("expected utility")
pf2w2 <- ggplot(data = df2m %>% filter(algo %in% algset) %>% filter(phi == 0.6), aes(x=as.numeric(variable), y=value/numreps, color = algo, shape = algo)) + scale_color_manual(values = algcols)+ theme(text = element_text(size = 18)) + geom_point(size=3) +geom_line() + xlab("agent number") + ylab("expected utility")

du_orig <- create_util() 
du<- du_orig %>% filter(algo %in% algset) %>% filter((20*phi)%%1 == 0) # note ob  is max b - min b so we are computing expected value of egalitarian welfare gap, not order bias
du2<- du %>% group_by(algo, n, phi) %>% transmute(ob=ob,ub=ub,eb=eb,up=up,ep=ep,obp=obp,nb=nb^(1/n), er=1+(1-ub)*(n-1)) %>% summarize(nbm = mean(nb), obm = mean(ob), ubm = mean(ub), ebm = mean(eb), erm = mean(er))
p0n <- ggplot(data = du2 %>% filter(algo %in% algset) %>% filter(phi == 1.0), aes(x = n, y = nbm, color = algo, shape = algo)) + scale_color_manual(values = algcols)+ theme(text = element_text(size = 18)) + geom_point(size=3) + geom_line() +ggtitle("") + xlab("number of agents") + ylab("Nash welfare") + guides(algo = guide_legend(order=1))
p1n <- ggplot(data = du2 %>% filter(algo %in% algset) %>% filter(phi == 0.6), aes(x = n, y = nbm, color = algo, shape = algo)) + scale_color_manual(values = algcols)+ theme(text = element_text(size = 18)) + geom_point(size=3) + geom_line() +ggtitle("") + xlab("number of agents") + ylab("Nash welfare") + guides(algo = guide_legend(order=1))
p2n <- ggplot(data = du2 %>% filter(algo %in% algset) %>% filter(n == 32), aes(x = phi, y = nbm, color = algo, shape = algo)) + scale_color_manual(values = algcols)+ theme(text = element_text(size = 18)) + geom_point(size=3) + geom_line() +ggtitle("") + xlab("Mallows parameter") + ylab("Nash welfare") + guides(algo = guide_legend(order=1))
p0w <- ggplot(data = du2 %>% filter(algo %in% algset)  %>% filter(phi == 1.0), aes(x = n, y = ubm, color = algo, shape = algo)) + scale_color_manual(values = algcols)+ theme(text = element_text(size = 18)) + geom_point(size=3) + geom_line() +ggtitle("") + xlab("number of agents") + ylab("utilitarian welfare") + guides(algo = guide_legend(order=1))
p1w <- ggplot(data = du2 %>% filter(algo %in% algset) %>% filter(phi == 0.6), aes(x = n, y = ubm, color = algo, shape = algo)) + scale_color_manual(values = algcols)+ theme(text = element_text(size = 18)) + geom_point(size=3) + geom_line() +ggtitle("") + xlab("number of agents") + ylab("utilitarian welfare") + guides(algo = guide_legend(order=1))
p2w <- ggplot(data = du2 %>% filter(algo %in% algset) %>% filter(n == 32), aes(x = phi, y = ubm, color = algo, shape = algo)) + scale_color_manual(values = algcols)+ theme(text = element_text(size = 18)) + geom_point(size=3) + geom_line() +ggtitle("") + xlab("Mallows parameter") + ylab("utilitarian welfare") + guides(algo = guide_legend(order=1))
p0o <- ggplot(data = du2 %>% filter(algo %in% algset) %>% filter(phi == 1.0), aes(x = n, y = obm, color = algo, shape = algo)) + scale_color_manual(values = algcols)+ theme(text = element_text(size = 18)) + geom_point(size=3) + geom_line() +ggtitle("") + xlab("number of agents") + ylab("order bias") + guides(algo = guide_legend(order=1))
p1o <- ggplot(data = du2 %>% filter(algo %in% algset) %>% filter(phi == 0.6), aes(x = n, y = obm, color = algo, shape = algo)) + scale_color_manual(values = algcols)+ theme(text = element_text(size = 18)) + geom_point(size=3) + geom_line() +ggtitle("") + xlab("number of agents") + ylab("order bias") + guides(algo = guide_legend(order=1))
p2o <- ggplot(data = du2 %>% filter(algo %in% algset) %>% filter(n == 32), aes(x = phi, y = obm, color = algo, shape = algo)) + scale_color_manual(values = algcols)+ theme(text = element_text(size = 18)) + geom_point(size=3) + geom_line() +ggtitle("") + xlab("Mallows parameter") + ylab("order bias") + guides(algo = guide_legend(order=1))
p2r <- ggplot(data = du2 %>% filter(algo %in% algset) %>% filter(n == 32), aes(x = phi, y = erm, color = algo, shape = algo)) + scale_color_manual(values = algcols)+ theme(text = element_text(size = 18)) + geom_point(size=3) + geom_line() +ggtitle("") + xlab("Mallows parameter") + ylab("average rank") + guides(algo = guide_legend(order=1))
