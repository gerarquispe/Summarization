coeficientes = [];
format shortE

% Leer file con el numero de trayectorias
[m ,dim] = textread('numberTrajectory.txt','%d %d');
fprintf('m=%d,dim=%d\n',m,dim);
file = fopen('idTrajectories.txt');

for i=0:m-1
    fprintf('%d\n',i)
    tline = fgetl(file);
    x = str2num(tline);
    %disp(x)
    tline = fgetl(file);
    y = str2num(tline);
    %disp(y)
    
    sz = length(x)
    time=(0:1:sz-1);
    %sz2 = length(time)
    sz3 = length(y)
    %%%%%%%%%%%%%%%%%%%%%%%%% Primer Código %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
       [tData,xData] = prepareCurveData(time,x);
       [tData,yData] = prepareCurveData(time,y);
       % Set up fittype and options.
       ft = fittype( 'poly5' );

       % Fit model to data. Elimina duplicados y centraliza
       [fitresult1, gof1] = fit( tData, xData, ft, 'Normalize', 'on' );
       [fitresult2, gof2] = fit( tData, yData, ft, 'Normalize', 'on' );
       
       %%%%%%%%%%%%%%%%%%%%%%%%%%% Image %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
       % Plot fit with data1.
       variable = figure( 'Name', 't vs x' );
       h = plot( fitresult1, tData, xData);
       legend( h, 'Point (t,x)', ' X fit line', 'Location', 'NorthEast' );
       % Label axes
       xlabel( 't' );
       ylabel( 'x' );
       grid on
       saveas(variable,strcat('interpolaciones/legendX.png')) % para escribir imagen
       %close
       
       %saveas(variable,strcat('interpolaciones/legendX.fig')) % para escribir imagen
       close
       
       % Plot fit with data2.
       variable2 = figure( 'Name', 't vs y' );
       h = plot( fitresult2, tData, yData );
       legend( h, 'Point (t,y)', 'Y fit line', 'Location', 'NorthEast' );
       % Label axes
       xlabel( 't' );
       ylabel( 'y' );
       grid on
       saveas(variable2,strcat('interpolaciones/legendY.png')) % para escribir imagen
       %close
       %saveas(variable2,strcat('interpolaciones/legendY.fig')) % para escribir imagen
       close
       
       names1 = strcat('interpolaciones/legendX.fig');
       names2 = strcat('interpolaciones/legendY.fig');
       % Load saved figures
       c=hgload(names1);
       k=hgload(names2);
       % Prepare subplots
       imagenSalida = figure
       xlabel( 't' );
       ylabel( 'x' );
       grid on
       
       h(1)=subplot(1,2,1);
       h(2)=subplot(1,2,2);
       % Paste figures on the subplots
       copyobj(allchild(get(c,'CurrentAxes')),h(1));
       copyobj(allchild(get(k,'CurrentAxes')),h(2));
       % Add legends
       l(1)=legend(h(1),'t vs X',' X fit line', 'Location', 'NorthEast');
       l(2)=legend(h(2),'t vs Y','Y fit line', 'Location', 'NorthEast');
       xlabel( 't' );
       ylabel( 'y' );
       grid on
       
       saveas(imagenSalida,strcat('interpolaciones/legend',int2str(i),'XY.png')) % para escribir imagen
       close
       
       %%%%%%%%%%%%%%%%%%%%% Fin Image %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
       
       %fitresult;
       %gof;
        
       %coeffnames(fitresult)
       a=coeffvalues(fitresult1) 
       coeficientes=cat(1,coeficientes,a);
       %coeficientes.append(a)
       %formula(fitresult) %muestra la formula del polinomio
       b=coeffvalues(fitresult2) 
       coeficientes=cat(1,coeficientes,b);       
end
fclose(file);
coeficientes
csvwrite('coeficientes.txt', coeficientes);

% Codigo para escribir coeficientes
% 
% fileID = fopen('coeficientes.txt','wt')
% fwrite(fileID,coeficientes);
% 
% % for ii = 1:size(coeficientes,1)
% %     fprintf(fileID,'%.6lf\t',coeficientes(ii,:))
% %     fprintf(fileID,'\n')
% % end
% fclose(fileID)

  %fprintf(fileID,'primera fila 3\r\n')
  %fprintf(fileID,'%lf %lf %lf %lf %lf %lf \r\n',coeficientes)
  %fclose(fileID)
