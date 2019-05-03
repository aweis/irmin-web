open TestFramework;

describe("other test suite", ({test}) =>
  test("failing test", ({expect}) => {
    expect.bool(true).toBeFalse();
    ();
  })
);
