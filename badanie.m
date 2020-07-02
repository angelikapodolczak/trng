id=fopen("strumien_act.txt","r");
x=fscanf(id,"%i ");
y=x';
fclose(id);

%%% HISTOGRAM
hist(y,255);

%%% ENROPIA
%%% e = -E( pi*log2(pi))

suma = 0;
N = 32768;

for i = 1:256   %przejscie po wartosciach probek
    n = 0;
    
    for j = 1:N    %przejscie po tablicy probek
        if y(j) == i
            n = n + 1;  %ilosc wystapien danej probki
        end
    end
    p = n / N; %prawdopodobienstwo wystapienia probki
    if p ~= 0
        suma = ( p * log2(p) ) + suma;
    end
end

entropia = -1 * suma
