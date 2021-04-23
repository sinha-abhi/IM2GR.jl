# order matters
data = Vector{Any}()
push!(data, UInt8.(cat([0 1; 1 1], [1 2; 0 0], dims=3)))
push!(data, UInt8.(cat([0 1 2 1 0; 1 1 1 1 1; 0 0 0 0 0], 
                       [1 2 3 4 5; 0 0 252 0 0; 1 1 1 1 1], dims=3)))
push!(data, UInt8.(cat([0 1 2 1 0; 1 1 1 1 1; 0 0 0 0 0], 
                       [1 2 3 4 5; 0 0 252 0 0; 1 1 1 1 1], dims=3)))
d = [1 1 2]
ret = [
  [56 13.86 11.31];
  [334 36.08 367031.79];
  [654 109.35 479834.24]
]

function run_construct_tests(mode::ConstructionMode, data, d, ret)
  for t = 1 : length(data)
    image = im2gr(data[t], d[t], mode)
    @test begin 
      length(image.ei) == Int(ret[t, 1]) && 
      length(image.ej) == Int(ret[t, 1])
    end
    @test begin
      isapprox(norm(image.evd), ret[t, 2], atol=0.02) && 
      isapprox(norm(image.evi), ret[t, 3], atol=0.02)
    end
  end

  nothing
end

@testset "single" begin
  run_construct_tests(IM2GR.CM_SingleThread, data, d, ret)
end

@testset "multi" begin
  run_construct_tests(IM2GR.CM_MultiThread, data, d, ret)
end

@testset "cuda" begin
  if CUDA.functional()
    for t = 1 : length(data)
      image = im2gr(data[t], d[t], IM2GR.CM_CUDA)
      @test begin
        isapprox(norm(filter(!isnan, image.V)), ret[t, 3], atol=0.2)
      end
    end
  else
    @warn("CUDA not available... ignoring CUDA tests")
  end

  nothing
end


@testset "consistency" begin
  fake = rand(UInt8, (144, 144, 22))
  st = im2gr(fake, 1, IM2GR.CM_SingleThread)
  mt = im2gr(fake, 1, IM2GR.CM_MultiThread)

  @test length(st.ei) == length(mt.ei) && length(st.ej) == length(mt.ej)
  @test norm(st.evd) ≈ norm(mt.evd) && norm(st.evi) ≈ norm(mt.evi)
end

