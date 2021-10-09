tests = [ {'description': 'Unbalanced 100',
    'steps': [ {'inputs': [('PINA',0x00), ('PINB',0x00),('PINC',100)], 'iterations': 5 } ],
    'expected': [('PORTD',102)],
    },
    {'description': 'Balanced 200',
    'steps': [ {'inputs': [('PINA',100), ('PINB',0x00),('PINC',100)], 'iterations': 5 } ],
    'expected': [('PORTD',201)],
    },
    {'description': 'Unbalanced 200',
    'steps': [ {'inputs': [('PINA',150), ('PINB',0x00),('PINC',50)], 'iterations': 5 } ],
    'expected': [('PORTD',203)],
    },
    {'description': 'normal',
    'steps': [ {'inputs': [('PINA',30), ('PINB',20),('PINC',30)], 'iterations': 5 } ],
    'expected': [('PORTD',80)],
    },
    ]
#watch = ['PORTD']

