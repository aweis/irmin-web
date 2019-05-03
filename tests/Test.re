open TestFramework;

describe("some test", ({test}) =>
  test("hello world", ({expect}) =>
    expect.bool(true).toBeTrue()
  )
);
