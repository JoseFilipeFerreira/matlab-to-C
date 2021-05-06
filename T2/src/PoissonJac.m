function [u,ITER]=PoissonJac(N, TOL)
% Finds the steady-state solution for the temperature distribution on a 
% square plate, for particular boundary conditions
% USES JACOBI ITERATIONS
% N is the number of grid points on each direction (including boundary
% points)
% TOL is the tolerance for the stopping citeria
% IN THE GRID, USES i FOR ROW (BOTTOM-UP), j FOR COLUMN (LEFT-RIGHT)


% SET BOUNDARY VALUES 
% Temperature is zero at top and 100 on the other boundaries
u(1,1:N)=100;  % lower boundary 
u(1:N,1)=100;  % left boundary 
u(1:N,N)=0;  % right boundary 
u(N,1:N)=0;    % boundary above

% w are the values u in the new iteration
w(1,1:N)=100;  % lower boundary 
w(1:N,1)=100;  % left boundary 
w(1:N,N)=0;  % right boundary 
w(N,1:N)=0;    % boundary above

% initial values for interior points

u(2:N-1,2:N-1)=50;

% COMPUTE STEADY STATE SOLUTION
DIFF=TOL+1;
ITER=0;
while DIFF>TOL
    for i=2:N-1
        for j=2:N-1
            w(i,j)=(u(i-1,j)+u(i,j-1)+u(i,j+1)+u(i+1,j))/4;
        end
    end
    DIFF=max(max(abs(w-u)));
    u=w; 
    ITER=ITER+1;
end

% DISPLAYS A COLOURFUL MAP OF DISTRIBUTIONS
u=flipud(u); image(u)
colorbar





